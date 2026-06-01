#pragma once

#include <stdio.h>

struct chapter {
	struct heading* title;
	int start_line;
	int end_line;
	char* body;
	struct chapter* next;
	struct chapter* first_child;
	struct chapter* parent;
};

int edit_chapter(const char* file_path, const char* chapter);
void free_chapter_tree(struct chapter* node);
struct chapter* parse_chapters(const char* source_code);
void search_chapters_for_str(const char* source_code, const char* str,
		FILE* stream);
void print_chapter(const char* source_code, const char* chapter, FILE* stream);
void print_chapter_no_color(const char* source_code, const char* chapter, FILE* stream);
void print_chapter_with_color(const char* source_code, const char* chapter, FILE* stream);
bool use_color(void);
