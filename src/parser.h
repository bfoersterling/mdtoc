#pragma once

#include <stdio.h> /* for FILE type */

struct heading {
	char* text;
	int level;
	int levels[6];
	int line;
	struct heading* next;
	struct heading* first_child;
	struct heading* parent;
};

char* atx_string(char* text, int level);
char* atx_string_from_heading(struct heading* h);
void find_heading_by_numbering(struct heading* node, const char* pretty_numbering, struct heading** result);
void free_heading_tree(struct heading* head);
struct heading* last_heading(struct heading* node);
struct heading* parse_headings(const char* source_code);
struct heading* parse_headings_from_stream(FILE* source_file);
char* pretty_heading_levels(int levels[6]);
void print_colored_markdown(const char* source_code, FILE* stream);
void print_heading(struct heading* h, int indentation_level, FILE* stream);
void print_heading_tree(struct heading* head, int recursion_level, FILE* stream);
void print_toc(const char* file_path, FILE* output_stream);
