#pragma once

#include <stdio.h>

int edit_chapter(const char* file_path, const char* chapter);
void print_chapter(FILE* source_file, const char* chapter, FILE* stream);
void print_chapter_no_color(const char* source_code, const char* chapter, FILE* stream);
void print_chapter_with_color(const char* source_code, const char* chapter, FILE* stream);
bool use_color(void);
