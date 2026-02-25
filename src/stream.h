#pragma once

int count_lines(FILE* stream);
long file_size(FILE* stream);
char* get_user_input(void);
long line_end_pos(FILE* source_file, int line);
long line_start_pos(FILE* source_file, int line);
char* read_file(const char* file_path);
char* read_file_section(FILE* source_file, long from, long to);
