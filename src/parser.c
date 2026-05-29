#include <assert.h>
#include <cmark.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "pretty_printer.h"
#include "stream.h"
#include "string_util.h"

// Static function prototypes.
static void append_heading(struct heading** head, const char* text, int level, int levels[6], int line);
static void concat_all_literals(cmark_node* node, char* buffer);
static char* extract_heading_text(cmark_node* node);
static struct heading* find_connector(struct heading* node, int level);
static void insert_preamble_heading(struct heading** head, const char* source_code);
static void parse_headings_from_node(cmark_node* node, struct heading** head, int levels[6]);
static void print_indentation(int level, FILE* stream);
static void push_heading(struct heading** head, const char* text, int level, int levels[6], int line);
static void update_heading_levels(int current_level, int levels[6]);

	static void
append_heading(struct heading** head, const char* text, int level, int levels[6], int line)
{
	assert(text != NULL);

	// Tree is empty.
	if (*head == NULL) {
		push_heading(head, text, level, levels, 0);
		return;
	}

	struct heading* connector = find_connector(*head, level);

	struct heading* new_node = malloc(sizeof(struct heading));
	size_t text_size = strlen(text) + 1;
	new_node->text = malloc(text_size);
	memset(new_node->text, 0, text_size);
	strcpy(new_node->text, text);
	new_node->level = level;
	memset(new_node->levels, 0, sizeof(int) * 6);
	memcpy(new_node->levels, levels, sizeof(int) * 6);
	new_node->line = line;
	new_node->first_child = NULL;
	new_node->next = NULL;

	if (connector->level < new_node->level) {
		assert(connector->first_child == NULL);
		connector->first_child = new_node;
		new_node->parent = connector;
		return;
	}

	if (connector == *head) {
		fprintf(stderr, "The root of a tree may NOT have a sibling.\n");
		exit(1);
	}

	assert(connector->next == NULL);
	new_node->parent = connector->parent;
	connector->next = new_node;
}

/*
 * Build an ATX heading string from a heading "text" and its "level".
 * "text" may not contain leading pound signs (#) or trailing setext elements.
 * Caller has to free the returned buffer.
 */
	char*
atx_string(char* text, int level)
{
	assert(text != NULL);
	assert(level <= 6);
	assert(level >= 1);

	// 8 bytes: max. 6 pound signs, 1 space, 1 null terminator
	size_t buffer_size = strlen(text) + 8;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);
	memset(buffer, '#', level);
	memset(buffer+level, ' ', 1);
	strcat(buffer, text);

	return buffer;
}

/*
 * See "atx_string()". (Caller has to free the returned buffer.)
 */
	char*
atx_string_from_heading(struct heading* h)
{
	return atx_string(h->text, h->level);
}

// Traverse node and concatenate the literals from all children and siblings
// in "buffer".
// The caller has to provide an allocated buffer "buffer" with enough space.
	static void
concat_all_literals(cmark_node* node, char* buffer)
{
	const char* literal = cmark_node_get_literal(node);

	if (literal != NULL)
		strcat(buffer, literal);

	if (cmark_node_first_child(node) != NULL)
		concat_all_literals(cmark_node_first_child(node), buffer);

	if (cmark_node_next(node) != NULL)
		concat_all_literals(cmark_node_next(node), buffer);
}

// Extracts only the literals from the heading for the toc.
// Caller has to free the returned buffer.
	static char*
extract_heading_text(cmark_node* node)
{
	assert(cmark_node_get_type(node) == CMARK_NODE_HEADING);

	size_t buffer_size = LINE_MAX;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	concat_all_literals(cmark_node_first_child(node), buffer);

	return buffer;
}

// "node" should be the root of the tree.
// "level" is the level of the new heading that will be attached to the
// connector.
	static struct heading*
find_connector(struct heading* node, int level)
{
	if (node->next != NULL)
		node = find_connector(node->next, level);

	if (node->first_child != NULL && node->level < level)
		node = find_connector(node->first_child, level);

	return node;
}

// Find "needle_numbering" (i.e. "2.1.") in heading tree.
// "node" should be the root of the tree.
// "result" should be a non-allocated heading pointer pointing to NULL.
// If no heading matches "result" remains NULL otherwise it points to the
// heading found.
// (To improve readability and code style, performance is sacrifized here.
// Performance could be improved by replacing recursion with loops and break
// on discovery.)
	void
find_heading_by_numbering(
		struct heading* node,
		const char* needle_numbering,
		struct heading** result
		)
{
	assert(node != NULL);

	char* node_numbering = pretty_heading_levels(node->levels);

	// The artificial root node with level 0 should not be found.
	// If an artificial preamble node exists however that should be found.
	if (strcmp(node_numbering, needle_numbering) == 0
			&& node->level != 0) {
		*result = node;
	}

	free(node_numbering);

	if (node->first_child)
		find_heading_by_numbering(node->first_child, needle_numbering, result);

	if (node->next)
		find_heading_by_numbering(node->next, needle_numbering, result);
}

	void
free_heading_tree(struct heading* node)
{
	if (node->first_child != NULL)
		free_heading_tree(node->first_child);

	if (node->next != NULL)
		free_heading_tree(node->next);

	free(node->text);
	free(node);
}

// Insert artificial heading for everything before the first heading
// unless there is no text.
// This function has to be called AFTER all headings have been parsed
// and the heading tree has already been generated.
	static void
insert_preamble_heading(struct heading** head, const char* source_code)
{
	if (*head == NULL)
		return;

	struct heading* first_heading = (*head)->first_child;

	if (first_heading != NULL && first_heading->line == 1)
		return;

	// No need for a preamble when the markdown file is empty.
	if (strlen(source_code) == 0)
		return;

	struct heading* new_node = malloc(sizeof(struct heading));
	// Give the artificial heading the same level as the first real heading
	// (which is heading number 1.).
	// If there are no headings at all -> level 1 because only the root of the
	// tree should be level 0.
	new_node->level = 1;

	if (first_heading != NULL)
		new_node->level = first_heading->level;

	memset(new_node->levels, 0, sizeof(int)*6);
	// The root node is already all zeros...
	// And the levels field is a unique value...
	memcpy(new_node->levels, (int[]){0,0,0,0,0,0}, sizeof(int)*6);
	new_node->line = 1;
	// We need to allocate this literal string because all other nodes allocate
	// this member and you cannot free a literal.
	const char* new_node_text = "preamble";
	size_t new_node_text_size = strlen(new_node_text) + 1;
	new_node->text = malloc(new_node_text_size);
	memset(new_node->text, 0, new_node_text_size);
	strcpy(new_node->text, new_node_text);
	new_node->first_child = NULL;
	new_node->next = first_heading;
	new_node->parent = *head;

	(*head)->first_child = new_node;
}

// UNUSED
// Return the last node of a tree branch.
// "node" is the parent of the (sub)tree.
// If "node" is the root of the tree this should be the bottom right node
// of the entire tree.
// If "node" is not the root node, it will return the last node of the
// subtree.
	struct heading*
last_heading(struct heading* node)
{
	if (node->first_child != NULL)
		node = last_heading(node->first_child);

	if (node->next != NULL)
		node = last_heading(node->next);

	return node;
}

// Get the root/head node of a tree with headings from the source code.
	struct heading*
parse_headings(const char* source_code)
{
	cmark_node* root_node = cmark_parse_document(source_code, strlen(source_code), 0);

	struct heading* head = NULL;

	int hlevels[6] = {0,0,0,0,0,0};

	// Adding an artificial root heading to make it a tree.
	push_heading(&head, "", 0, hlevels, 0);

	parse_headings_from_node(root_node, &head, hlevels);

	insert_preamble_heading(&head, source_code);

	cmark_node_free(root_node);

	return head;
}

	static void
parse_headings_from_node(cmark_node* node, struct heading** head, int hlevels[6])
{
	if (cmark_node_get_type(node) == CMARK_NODE_HEADING) {
		int hlevel = cmark_node_get_heading_level(node);
		update_heading_levels(hlevel, hlevels);
		char* heading_literal = extract_heading_text(node);

		append_heading(head,
				heading_literal,
				hlevel,
				hlevels,
				cmark_node_get_start_line(node));

		free(heading_literal);
	}

	if (cmark_node_first_child(node) != NULL)
		parse_headings_from_node(cmark_node_first_child(node), head, hlevels);

	if (cmark_node_next(node) != NULL)
		parse_headings_from_node(cmark_node_next(node), head, hlevels);
}

// Read markdown file from an open stream instead of from a char pointer.
	struct heading*
parse_headings_from_stream(FILE* source_file)
{
	long initial_file_pos = ftell(source_file);
	size_t file_content_size = file_size(source_file) + 1;
	char* file_content = malloc(file_content_size);
	memset(file_content, 0, file_content_size);

	fseek(source_file, 0, SEEK_SET);
	fread(file_content, file_content_size, 1, source_file);
	fseek(source_file, initial_file_pos, SEEK_SET);

	struct heading* root = parse_headings(file_content);

	free(file_content);

	return root;
}

// Caller has to free the returned buffer.
	char*
pretty_heading_levels(int levels[6])
{
	size_t buffer_size = 512;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	for (int i = 0; i < 6; i++) {
		if (levels[i] == 0)
			continue;

		char* level_str = str_from_int(levels[i]);
		strcat(buffer, level_str);
		strcat(buffer, ".");
		free(level_str);
	}

	// Check for preamble heading.
	if (memcmp(levels, (int[]){0,0,0,0,0,0}, sizeof(int)*6) == 0
			&& strcmp(buffer, "") == 0)
		strcpy(buffer, "0.");

	return buffer;
}

	void
print_heading_tree(struct heading* node, int recursion_level, FILE* stream)
{
	print_heading(node, recursion_level, stream);

	if (node->first_child != NULL)
		print_heading_tree(node->first_child, recursion_level+1, stream);

	if (node->next != NULL)
		print_heading_tree(node->next, recursion_level, stream);
}

/*
 * Prints heading "h" with indentation "indentation_level" to "stream".
 * Prints the heading numbering, its text and its line.
 * Headings with level 0 will not be printed.
 */
	void
print_heading(struct heading* h, int indentation_level, FILE* stream)
{
	// Do not print the artificial root.
	if (h->level == 0)
		return;

	print_indentation(indentation_level-1, stream);
	pretty_print_heading_levels(h->levels, stream);
	fprintf(stream, " %s (%d)\n", h->text, h->line);
	fflush(stream);
}

	static void
print_indentation(int level, FILE* stream)
{
	for (int i = 0; i < level; i++) {
		fprintf(stream, "  ");
	}
	fflush(stream);
}

	void
print_toc(const char* file_path, FILE* output_stream)
{
	FILE* source_file = fopen(file_path, "r");

	if (source_file == NULL) {
		fprintf(output_stream, "Error opening file \"%s\": %s\n", file_path, strerror(errno));
		return;
	}

	struct heading* root = parse_headings_from_stream(source_file);

	print_heading_tree(root, 0, output_stream);

	free_heading_tree(root);

	fclose(source_file);
}

// Push a new node to the root of the tree.
	static void
push_heading(struct heading** head, const char* text, int level, int levels[6], int line)
{
	struct heading* new_node = malloc(sizeof(struct heading));
	size_t text_size = strlen(text) + 1;
	new_node->text = malloc(text_size);
	memset(new_node->text, 0, text_size);
	strcpy(new_node->text, text);
	new_node->level = level;
	memset(new_node->levels, 0, sizeof(int) * 6);
	memcpy(new_node->levels, levels, sizeof(int) * 6);
	new_node->line = line;
	new_node->next = NULL;
	new_node->first_child = *head;
	new_node->parent = NULL;

	if (*head != NULL)
		(*head)->parent = new_node;

	*head = new_node;
}

	static void
update_heading_levels(int current_level, int levels[6])
{
	assert(current_level >= 1 && current_level <= 6);
	levels[current_level-1]++;

	// clear lower levels
	for (int i = current_level; i < 6; i++) {
		levels[i] = 0;
	}
}
