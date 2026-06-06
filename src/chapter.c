#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "chapter.h"
#include "heading.h"
#include "pretty_printer.h"
#include "stream.h"
#include "string_util.h"

// Prototypes for static functions.
static int chapter_last_line(struct heading* chapter_heading, const char* source_code);
static struct chapter* find_chapter_by_numbering(struct chapter* node,
		const char* numbering);
static void parse_chapters_from_headings(struct chapter** head,
		struct chapter** c, struct heading* h, const char* source_code);
static void search_chapters_for_str_rec(struct chapter* node, const char* str,
		int level, FILE* stream);


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

	char* source_code = read_file(file_path, true);

	struct heading* root = parse_headings(source_code);

	struct heading* chapter_heading = NULL;

	char* numbering = numbering_with_trailing_dot(chapter);

	find_heading_by_numbering(root, numbering, &chapter_heading);

	if (chapter_heading == NULL) {
		fprintf(stderr, "Chapter %s was not found in %s.\n", numbering, file_path);
		free(source_code);
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

	free(line_argument);
	free(numbering);
	free_heading_tree(root);
	free(source_code);

	return 0;
}

/*
 * Finds the first chapter with a heading numbering "numbering".
 * "numbering" should have a trailing dot (i.e. "2.1.") so that this
 * recursive function does not have to call "numbering_with_trailing_dot()"
 * repeatedly and make a lot of allocations.
 * "node" should be the root of the tree or the node you want to start
 * your search at.
 */
	static struct chapter*
find_chapter_by_numbering(struct chapter* node, const char* numbering)
{
	struct chapter* needle_chapter = NULL;
	char* pretty_levels = pretty_heading_levels(node->title->levels);

	if (strcmp(pretty_levels, numbering) == 0
			&& node->title->level != 0) {
		free(pretty_levels);
		return node;
	}

	free(pretty_levels);

	if (node->first_child != NULL)
		needle_chapter = find_chapter_by_numbering(node->first_child, numbering);

	if (node->next != NULL && needle_chapter == NULL)
		needle_chapter = find_chapter_by_numbering(node->next, numbering);

	return needle_chapter;
}

// Free memory of "node" and all its children.
	void
free_chapter_tree(struct chapter* node)
{
	if (node->first_child != NULL)
		free_chapter_tree(node->first_child);

	if (node->next != NULL)
		free_chapter_tree(node->next);

	free(node->title->text);
	free(node->title);
	free(node->body);
	free(node);
}

/* Returns the root chapter with all its children.
 * The freeing function for chapters has to free the headings as well.
 * Caller has to free the chapter tree and the heading tree.
 * (preferably with free_chapters_with_headings())
 */
	struct chapter*
parse_chapters(const char* source_code)
{
	struct heading* root_heading = parse_headings(source_code);
	struct chapter* root_chapter = NULL;
	struct chapter* chapter_iterator = NULL;

	parse_chapters_from_headings(&root_chapter, &chapter_iterator,
			root_heading, source_code);

	return root_chapter;
}

/* Recursive function for parse_chapters().
 * "head" and "c" need to be pointers to NULL pointers of struct chapter.
 * (Cannot pass NULL directly here.)
 * "c" is needed to create the tree, but shall not be used before or
 * after this function.
 * "head" is the pointer to the root chapter.
 * "h" has to be the root heading.
 */
	static void
parse_chapters_from_headings(
		struct chapter** head,
		struct chapter** c,
		struct heading* h,
		const char* source_code)
{
	struct chapter* new_chapter = malloc(sizeof(struct chapter));

	// Construct artificial root.
	// The entire document is not a real chapter so its body is set to an
	// empty string.
	if (*head == NULL) {
		new_chapter->start_line = 0;
		new_chapter->end_line = 0;
		new_chapter->body = malloc(8);
		memset(new_chapter->body, 0, 8);
		*head = new_chapter;
	} else if (strcmp(h->text, "preamble") == 0) {
		// Preamble headings do not have a corresponding heading in the
		// markdown source, so we need to start at line 1 instead of adding
		// 1 to the heading line field.
		new_chapter->start_line = 1;
		new_chapter->end_line = chapter_last_line(h, source_code);
		new_chapter->body = string_line_span(source_code,
				new_chapter->start_line, new_chapter->end_line);
	} else {
		new_chapter->start_line = h->line;
		new_chapter->end_line = chapter_last_line(h, source_code);
		if (new_chapter->end_line > new_chapter->start_line) {
			new_chapter->body = string_line_span(source_code,
					new_chapter->start_line + 1, new_chapter->end_line);
		} else {
			// There is no body, the chapter consists only of the heading.
			new_chapter->body = malloc(8);
			memset(new_chapter->body, 0, 8);
		}
	}

	new_chapter->title = h;
	new_chapter->next = NULL;
	new_chapter->first_child = NULL;
	new_chapter->parent = NULL;

	*c = new_chapter;

	if (h->next != NULL) {
		parse_chapters_from_headings(head, &((*c)->next), h->next, source_code);
	}

	if (h->first_child != NULL) {
		parse_chapters_from_headings(head, &((*c)->first_child), h->first_child, source_code);
	}
}

/*
 * Prints chapter "chapter" from string "source_code".
 * "source_code": The content of a markdown file.
 * "chapter": i.e. "1.2." or "1.2" (trailing dot will be appended).
 * "stream": Output stream. (i.e. stdout or string stream for tests)
 */
	void
print_chapter(const char* source_code, const char* chapter, FILE* stream)
{
	assert(source_code != NULL);
	assert(chapter != NULL);

	if (*chapter == '\0') {
		fprintf(stream, "Provided chapter was an empty string.\n");
		return;
	}

	if (*source_code == '\0') {
		fprintf(stream, "Source code is an empty string.\n");
		return;
	}

	char* dotted_numbering = numbering_with_trailing_dot(chapter);

	struct chapter* root_chapter = parse_chapters(source_code);

	struct chapter* needle_chapter =
		find_chapter_by_numbering(root_chapter, dotted_numbering);

	if (needle_chapter == NULL) {
		fprintf(stream, "Chapter %s could not be found.\n", dotted_numbering);
		goto end;
	}

	char* heading_str = atx_string_from_heading(needle_chapter->title);

	if (use_color())
		print_colored_markdown(heading_str, stream);
	else
		fprintf(stream, "%s", heading_str);

	// Add newline between heading and chapter.
	fprintf(stream, "\n");

	assert(needle_chapter->body != NULL);

	if (use_color()) {
		print_colored_markdown(needle_chapter->body, stream);
	} else {
		fprintf(stream, "%s", needle_chapter->body);
	}

	free(heading_str);
end:
	free(dotted_numbering);
	free_chapter_tree(root_chapter);
	fflush(stream);
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

/*
 * Prints chapters in "source_code" that contain "str" to "stream".
 * (see search_chapters_for_str_rec() for details)
 */
	void
search_chapters_for_str(const char* source_code, const char* str, FILE* stream)
{
	struct chapter* root_chapter = parse_chapters(source_code);

	search_chapters_for_str_rec(root_chapter, str, 0, stream);

	free_chapter_tree(root_chapter);
}

/*
 * Recursive function that prints the headings of chapters that contain the
 * string "str" in either the heading text or in the body of the chapter.
 * The search is case insensitive.
 * The headings are printed with indentation level, text and line.
 * The output goes to "stream".
 */
	static void
search_chapters_for_str_rec(
		struct chapter* node,
		const char* str,
		int level,
		FILE* stream)
{
	if (node->title->text != NULL && node->body != NULL)
		if (strcasestr(node->title->text, str) != NULL ||
				strcasestr(node->body, str) != NULL)
			print_heading(node->title, level, stream);

	if (node->first_child != NULL)
		search_chapters_for_str_rec(node->first_child, str, level+1, stream);

	if (node->next != NULL)
		search_chapters_for_str_rec(node->next, str, level, stream);
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
