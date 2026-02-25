#pragma once

#include <stdio.h> /* for FILE type */

struct heading {
	char* text;
	int level;
	int levels[6];
	int line;
	struct heading* first_child;
	struct heading* next;
	struct heading* parent;
};

void find_heading_by_numbering(struct heading* node, const char* pretty_numbering, struct heading** result);
void free_heading_tree(struct heading* head);
struct heading* last_heading(struct heading* node);
struct heading* parse_headings(const char* source_code);
struct heading* parse_headings_from_stream(FILE* source_file);
char* pretty_heading_levels(int levels[6]);
void print_ast(const char* source_code);
void print_colored_markdown(const char* source_code, FILE* stream);
void print_heading_tree(struct heading* head, int recursion_level, FILE* stream);
void print_toc(const char* file_path, FILE* output_stream);
