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

	static long
chapter_end_pos(FILE* source_file, struct heading* node)
{
	assert(node != NULL);
	assert(source_file != NULL);

	if (node->next != NULL)
		return line_start_pos(source_file, node->next->line);

	// Last chapter of the file.
	if (node->next == NULL && node->parent->next == NULL) {
		return file_size(source_file);
	} else {
		return line_start_pos(source_file, node->parent->next->line);
	}

	return -1;
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
		fprintf(stderr, "Error opening file %s: %s\n", file_path, strerror(errno));
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

// Caller has to free the returned buffer.
// Returns the "chapter" in "source_file" or NULL if it could not be found.
	char*
fetch_chapter(FILE* source_file, const char* chapter)
{
	assert(source_file != NULL);

	if (chapter == NULL)
		return NULL;

	assert(*chapter != '\0');

	char* numbering = numbering_with_trailing_dot(chapter);
	struct heading* root = parse_headings_from_stream(source_file);
	struct heading* chapter_heading = NULL;

	find_heading_by_numbering(root, numbering, &chapter_heading);

	if (chapter_heading == NULL) {
		free_heading_tree(root);
		free(numbering);
		return NULL;
	}

	long start_pos = line_start_pos(source_file, chapter_heading->line);
	long end_pos = chapter_end_pos(source_file, chapter_heading);

	assert(start_pos >= 0);

	free_heading_tree(root);
	free(numbering);

	return read_file_section(source_file, start_pos, end_pos);
}

void
print_chapter(FILE* source_file, const char* chapter, FILE* stream)
{
	if (use_color())
		print_chapter_with_color(source_file, chapter, stream);
	else
		print_chapter_no_color(source_file, chapter, stream);
}

	void
print_chapter_no_color(FILE* source_file, const char* chapter, FILE* stream)
{
	if (*chapter == '\0') {
		fprintf(stream, "Provided chapter was an empty string.\n");
		return;
	}
	assert(source_file != NULL);
	assert(stream != NULL);

	struct heading* root = parse_headings_from_stream(source_file);
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

	long start_pos = line_start_pos(source_file, chapter_heading->line);
	long end_pos = chapter_end_pos(source_file, chapter_heading);

	assert(start_pos >= 0);

	char* section = read_file_section(source_file, start_pos, end_pos);

	fprintf(stream, "%s", section);
	fflush(stream);

	free(section);
	free(numbering);
	free_heading_tree(root);
}

	void
print_chapter_with_color(FILE* source_file, const char* chapter, FILE* stream)
{
	if (*chapter == '\0') {
		fprintf(stream, "Provided chapter was an empty string.\n");
		return;
	}
	assert(source_file != NULL);
	assert(stream != NULL);

	size_t source_size = file_size(source_file) + 1;
	char* source = malloc(source_size);
	memset(source, 0, source_size);

	fread(source, source_size, 1, source_file);

	struct heading* root = parse_headings_from_stream(source_file);
	assert(root != NULL);
	struct heading* chapter_heading = NULL;

	char* numbering = numbering_with_trailing_dot(chapter);

	find_heading_by_numbering(root, numbering, &chapter_heading);

	if (chapter_heading == NULL) {
		fprintf(stream, "Chapter %s could not be found.\n", numbering);
		free(numbering);
		free(source);
		free_heading_tree(root);
		return;
	}

	long start_pos = line_start_pos(source_file, chapter_heading->line);
	long end_pos = chapter_end_pos(source_file, chapter_heading);

	assert(start_pos >= 0);

	char* section = read_file_section(source_file, start_pos, end_pos);

	print_colored_markdown(section, stream);

	free(source);
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
