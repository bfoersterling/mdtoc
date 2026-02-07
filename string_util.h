#pragma once

void ensure_trailing_dot(char** buffer);
const char* first_digit_in_str(const char* s);
char* numbering_with_trailing_dot(const char* s);
char* merge_strings(const char* s1, const char* s2);
void print_by_line_column(const char* s, int from_line, int from_col,
		int to_line, int to_col, FILE* stream);
char* str_from_int(int num);
char* string_line_span(const char* s, int from_line, int to_line);
void trim_space(char* s);
