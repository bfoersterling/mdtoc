#include <assert.h>
#include <cmark.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "pretty_printer.h"
#include "stream.h"
#include "string_util.h"

// Static function prototypes.
static void append_heading(struct heading** head, char* text, int level, int levels[6], int line);
static struct heading* find_connector(struct heading* node, int level);
static void parse_headings_from_node(cmark_node* node, struct heading** head, int levels[6]);
static void print_colored_cmark_tree(cmark_node* node, const char* source_code, FILE* stream);
static void print_heading(struct heading* h, int indentation_level, FILE* stream);
static void print_indentation(int level, FILE* stream);
static void print_node(cmark_node* node);
static void push_heading(struct heading** head, char* text, int level, int levels[6], int line);
static void traverse_tree(cmark_node* node, int level);
static void update_heading_levels(int current_level, int levels[6]);

static void
append_heading(struct heading** head, char* text, int level, int levels[6], int line) {
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

// "node" should be the root of the tree.
// "level" is the level of the new heading that will be attached to the
// connector.
static struct heading*
find_connector(struct heading* node, int level) {
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
		) {
	assert(node != NULL);

	char* node_numbering = pretty_heading_levels(node->levels);

	if (strcmp(node_numbering, needle_numbering) == 0) {
		*result = node;
	}

	free(node_numbering);

	if (node->first_child)
		find_heading_by_numbering(node->first_child, needle_numbering, result);

	if (node->next)
		find_heading_by_numbering(node->next, needle_numbering, result);
}

void
free_heading_tree(struct heading* node) {
	if (node->first_child != NULL)
		free_heading_tree(node->first_child);

	if (node->next != NULL)
		free_heading_tree(node->next);

	free(node->text);
	free(node);
}

// Return the last node of a tree branch.
// "node" is the parent of the (sub)tree.
// If "node" is the root of the tree this should be the bottom right node
// of the entire tree.
// If "node" is not the root node, it will return the last node of the
// subtree.
struct heading*
last_heading(struct heading* node) {
	if (node->first_child != NULL)
		node = last_heading(node->first_child);

	if (node->next != NULL)
		node = last_heading(node->next);

	return node;
}

// Get the root/head node of a tree with headings from the source code.
struct heading*
parse_headings(const char* source_code) {
	cmark_node* root_node = cmark_parse_document(source_code, strlen(source_code), 0);

	struct heading* head = NULL;

	int hlevels[6] = {0,0,0,0,0,0};

	// Adding an artificial root heading to make it a tree.
	push_heading(&head, "", 0, hlevels, 0);

	parse_headings_from_node(root_node, &head, hlevels);

	cmark_node_free(root_node);

	return head;
}

static void
parse_headings_from_node(cmark_node* node, struct heading** head, int hlevels[6]) {
	if (cmark_node_get_type(node) == CMARK_NODE_HEADING) {
		int hlevel = cmark_node_get_heading_level(node);
		update_heading_levels(hlevel, hlevels);
		append_heading(head,
				(char*)cmark_node_get_literal(cmark_node_first_child(node)),
				hlevel,
				hlevels,
				cmark_node_get_start_line(node));
	}

	if (cmark_node_first_child(node) != NULL)
		parse_headings_from_node(cmark_node_first_child(node), head, hlevels);

	if (cmark_node_next(node) != NULL)
		parse_headings_from_node(cmark_node_next(node), head, hlevels);
}

// Read markdown file from an open stream instead of from a char pointer.
struct heading*
parse_headings_from_stream(FILE* source_file) {
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
pretty_heading_levels(int levels[6]) {
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

	return buffer;
}

void
print_ast(const char* source_code) {
	cmark_node* root_node = cmark_parse_document(
			source_code,
			strlen(source_code),
			CMARK_OPT_DEFAULT);

	traverse_tree(root_node, 0);

	cmark_node_free(root_node);
}

	static void
print_colored_cmark_tree(cmark_node* node, const char* source_code, FILE* stream)
{
	cmark_node_type node_type = cmark_node_get_type(node);

	pretty_print_cmark_node(node, stream);

	fflush(stream);

	if (cmark_node_first_child(node))
		print_colored_cmark_tree(cmark_node_first_child(node), source_code, stream);

	if (node_type == CMARK_NODE_PARAGRAPH || node_type == CMARK_NODE_HEADING) {
		fputc('\n', stream);
	}

	if (cmark_node_next(node))
		print_colored_cmark_tree(cmark_node_next(node), source_code, stream);
}

	void
print_colored_markdown(const char* source_code, FILE* stream)
{
	cmark_node* cmark_root = cmark_parse_document(source_code, strlen(source_code), 0);

	print_colored_cmark_tree(cmark_root, source_code, stream);

	// Reset colors.
	fprintf(stream, "\033[0m");
	fflush(stream);

	cmark_node_free(cmark_root);
}

void
print_heading_tree(struct heading* node, int recursion_level, FILE* stream) {
	print_heading(node, recursion_level, stream);

	if (node->first_child != NULL)
		print_heading_tree(node->first_child, recursion_level+1, stream);

	if (node->next != NULL)
		print_heading_tree(node->next, recursion_level, stream);
}

static void
print_heading(struct heading* h, int indentation_level, FILE* stream) {
	// Do not print the artificial root.
	if (h->level == 0)
		return;

	print_indentation(indentation_level-1, stream);
	pretty_print_heading_levels(h->levels, stream);
	fprintf(stream, " %s (%d)\n", h->text, h->line);
	fflush(stream);
}

static void
print_indentation(int level, FILE* stream) {
	for (int i = 0; i < level; i++) {
		fprintf(stream, "  ");
	}
	fflush(stream);
}

static void
print_node(cmark_node* node) {
	printf("%s", cmark_node_get_type_string(node));
	printf(" [%d, %d] ",
			cmark_node_get_start_line(node),
			cmark_node_get_start_column(node));
	printf("- [%d, %d]\n",
			cmark_node_get_end_line(node),
			cmark_node_get_end_column(node));
}

void
print_toc(const char* file_path, FILE* output_stream) {
	FILE* source_file = fopen(file_path, "r");

	if (source_file == NULL) {
		fprintf(output_stream, "Error opening %s: %s\n", file_path, strerror(errno));
		return;
	}

	struct heading* root = parse_headings_from_stream(source_file);

	print_heading_tree(root, 0, output_stream);

	free_heading_tree(root);

	fclose(source_file);
}

// Push a new node to the root of the tree.
static void
push_heading(struct heading** head, char* text, int level, int levels[6], int line) {
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
traverse_tree(cmark_node* node, int level) {
	for (int i = 0; i<level; i++) {
		printf("  ");
	}

	print_node(node);

	cmark_node* child = cmark_node_first_child(node);

	while (child != NULL) {
		traverse_tree(child, level+1);
		child = cmark_node_next(child);
	}
}

static void
update_heading_levels(int current_level, int levels[6]) {
	assert(current_level >= 1 && current_level <= 6);
	levels[current_level-1]++;

	// clear lower levels
	for (int i = current_level; i < 6; i++) {
		levels[i] = 0;
	}
}
