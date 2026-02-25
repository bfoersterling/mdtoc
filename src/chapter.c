#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "chapter.h"
#include "parser.h"
#include "stream.h"
#include "string_util.h"

// Prototypes for static functions.
static int chapter_last_line(struct heading* chapter_heading, const char* source_code);

// Returns the line where the chapter that begins with "chapter_heading" ends.
// Returns -1 on error.
	static int
chapter_last_line(struct heading* chapter_heading, const char* source_code)
{
	if (chapter_heading == NULL)
		return -1;

	if (source_code == NULL || *source_code == '\0')
		return -1;

	for (struct heading* h = chapter_heading; h != NULL; h = h->parent) {
		if (h->next != NULL)
			return h->next->line - 1;
	}

	return string_line_count(source_code);
}

// Returns 0 on success, and 1 on error.
	int
edit_chapter(const char* file_path, const char* chapter)
{
	if (file_path == NULL) {
		fprintf(stderr, "No file to edit was passed.\n");
		return 1;
	}
	if (chapter == NULL) {
		fprintf(stderr, "No chapter to edit was passed.\n");
		return 1;
	}

	FILE* source_file = fopen(file_path, "r");

	if (source_file == NULL) {
		fprintf(stderr, "Error opening file \"%s\": %s\n", file_path, strerror(errno));
		return 1;
	}

	struct heading* root = parse_headings_from_stream(source_file);

	struct heading* chapter_heading = NULL;

	char* numbering = numbering_with_trailing_dot(chapter);

	find_heading_by_numbering(root, numbering, &chapter_heading);

	if (chapter_heading == NULL) {
		fprintf(stderr, "Chapter %s was not found in %s.\n", numbering, file_path);
		free(numbering);
		free_heading_tree(root);
		return 1;
	}

	int chapter_line = chapter_heading->line;

	const char* editor = getenv("EDITOR");

	if (editor == NULL)
		editor = "vim";

	char* line_buffer = str_from_int(chapter_line);

	char* line_argument = merge_strings("+", line_buffer);

	free(line_buffer);

	if (fork() == 0) {
		execlp(editor, editor, line_argument, file_path, (char*)NULL);
	}

	wait(0);

	free(numbering);
	free(line_argument);

	free_heading_tree(root);

	fclose(source_file);

	return 0;
}

	void
print_chapter(FILE* source_file, const char* chapter, FILE* stream)
{
	assert(source_file != NULL);

	size_t source_size = file_size(source_file) + 1;
	char* source = malloc(source_size);
	memset(source, 0, source_size);

	long initial_file_pos = ftell(source_file);
	fseek(source_file, 0, SEEK_SET);
	fread(source, source_size, 1, source_file);
	fseek(source_file, initial_file_pos, SEEK_SET);

	if (*chapter == '\0') {
		fprintf(stream, "Provided chapter was an empty string.\n");
		return;
	}
	if (use_color())
		print_chapter_with_color(source, chapter, stream);
	else
		print_chapter_no_color(source, chapter, stream);

	free(source);
}

	void
print_chapter_no_color(const char* source_code, const char* chapter, FILE* stream)
{
	assert(stream != NULL);

	struct heading* root = parse_headings(source_code);
	assert(root != NULL);
	struct heading* chapter_heading = NULL;

	char* numbering = numbering_with_trailing_dot(chapter);

	find_heading_by_numbering(root, numbering, &chapter_heading);

	if (chapter_heading == NULL) {
		fprintf(stream, "Chapter %s could not be found.\n", numbering);
		free(numbering);
		free_heading_tree(root);
		return;
	}

	long start_line = chapter_heading->line;
	long end_line = chapter_last_line(chapter_heading, source_code);

	assert(start_line > 0);
	assert(end_line > 0);

	char* section = string_line_span(source_code, start_line, end_line);

	if (section != NULL)
		fprintf(stream, "%s", section);

	fflush(stream);

	free(section);
	free(numbering);
	free_heading_tree(root);
}

	void
print_chapter_with_color(const char* source_code, const char* chapter, FILE* stream)
{
	assert(stream != NULL);

	struct heading* root = parse_headings(source_code);
	assert(root != NULL);
	struct heading* chapter_heading = NULL;

	char* numbering = numbering_with_trailing_dot(chapter);

	find_heading_by_numbering(root, numbering, &chapter_heading);

	if (chapter_heading == NULL) {
		fprintf(stream, "Chapter %s could not be found.\n", numbering);
		free(numbering);
		free_heading_tree(root);
		return;
	}

	long start_line = chapter_heading->line;
	long end_line = chapter_last_line(chapter_heading, source_code);

	assert(start_line > 0);
	assert(end_line > 0);

	char* section = string_line_span(source_code, start_line, end_line);

	if (section != NULL)
		print_colored_markdown(section, stream);

	free(numbering);
	free(section);
	free_heading_tree(root);
}

	bool
use_color(void)
{
	// no-color.org says to set NO_COLOR=1
	const char* no_color = getenv("NO_COLOR");

	if (no_color != NULL && strcmp(no_color, "1") == 0)
		return false;

	if (isatty(1))
		return true;

	return false;
}
