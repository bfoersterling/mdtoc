#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// UNUSED
// "buffer" needs to be an allocated string.
	void
ensure_trailing_dot(char** buffer)
{
	if (buffer == NULL || *buffer == NULL)
		return;

	// Trailing dot already present.
	if (**buffer != '\0' && *(*buffer+strlen(*buffer)-1) == '.')
		return;

	// Length of buffer + "." + null terminator.
	size_t new_buffer_size = strlen(*buffer) + 2;
	*buffer = realloc(*buffer, new_buffer_size);
	assert(*buffer != NULL);

	// initialize new memory
	memset(*buffer+strlen(*buffer), 0, 2);

	strcat(*buffer, ".");
}

// Returns a pointer to the first occurence of a digit in "s"
// or NULL if there is no digit in "s".
// No memory is allocated - don't free the return value.
	const char*
first_digit_in_str(const char* s)
{
	if (s == NULL)
		return NULL;

	for (const char* p = s; *p != '\0'; p++) {
		if (isdigit((int)*p))
			return p;
	}

	return NULL;
}

// Caller needs to free the returned buffer.
// 1.1 -> 1.1.
// 1.1. -> 1.1.
	char*
numbering_with_trailing_dot(const char* s)
{
	assert(s != NULL);

	size_t buffer_size = 2048;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);
	strcpy(buffer, s);

	if (*buffer == '\0' || buffer[strlen(buffer)-1] != '.') {
		strcat(buffer, ".");
	}

	return buffer;
}

// Caller has to free the returned buffer.
// The return value will contain a string that starts with s1 and ends with s2.
// s1 and s2 remain unchanged.
// s1 and s2 may be literal strings or pointers to allocated memory.
// Returns NULL on error.
	char*
merge_strings(const char* s1, const char* s2)
{
	if (s1 == NULL || s2 == NULL)
		return NULL;

	size_t buffer_size = strlen(s1) + strlen(s2) + 1;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	strcpy(buffer, s1);
	strcat(buffer, s2);

	return buffer;
}

// UNUSED
// cmark starts counting at 1, but the end col may be 0 if there is an
// empty line.
// We start at 0 for line and column.
	void
print_by_line_column(
		const char* s,
		int from_line, int from_col,
		int to_line, int to_col,
		FILE* stream
		)
{
	if (from_line < 0 || from_col < 0 || to_line < 0 || to_col < 0)
		return;

	char* duplicate = strdup(s);
	int line = 0;
	int col = 0;
	for (
			char* token = strtok(duplicate, "\r\n");
			token != NULL;
			token = strtok(NULL, "\r\n")
		) {
		if (line < from_line) {
			line++;
			continue;
		}

		for (char* p = token; *p != '\0'; p++) {
			if (col < from_col) {
				col++;
				continue;
			}

			if (line >= to_line && col >= to_col) {
				fflush(stream);
				free(duplicate);
				return;
			}

			fprintf(stream, "%c", *p);

			col++;
		}

		if (line >= from_line)
			fprintf(stream, "\n");

		col = 0;
		line++;
	}

	fflush(stream);
	free(duplicate);
}

// Caller has to free the returned buffer.
	char*
str_from_int(int num)
{
	// INT_MAX is 2147483647 in limits.h => 10 digits.
	// A buffer of 16 bytes should be enough for any int.
	size_t buffer_size = 16;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	sprintf(buffer, "%d", num);

	return buffer;
}

	int
string_line_end_byte(const char* s, int line)
{
	int index = 0;
	int cur_line = 1;

	for (char* p = (char*)s; *p != '\0'; p++) {
		if (cur_line == line
				&& (*p == '\n' || *(p+1) == '\0' ))
			return index;

		if (*p == '\n')
			cur_line++;

		index++;
	}

	return -1;
}

// Caller has to free the returned buffer.
// Return lines "from_line"-"to_line" from "s".
// The first line is line 1.
// Returns NULL if the line span could not be found in "s".
	char*
string_line_span(const char* s, int from_line, int to_line)
{
	if (s == NULL || *s == '\0')
		return NULL;

	if (from_line < 1 || from_line > to_line)
		return NULL;

	char* s_copy = strdup(s);
	size_t buffer_size = strlen(s) + 1;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	int line_number = 1;
	for(
			char* line = strtok(s_copy, "\r\n");
			line != NULL;
			line = strtok(NULL, "\r\n")
	   ) {
		if (line_number < from_line) {
			line_number++;
			continue;
		}

		if (line_number > to_line)
			break;

		strcat(buffer, line);
		strcat(buffer, "\n");

		line_number++;
	}

	free(s_copy);
	return buffer;
}

	int
string_line_start_byte(const char* s, int line)
{
	int cur_line = 1;

	for (int i = 0; i < strlen(s); i++) {
		if (cur_line == line)
			return i;

		if (s[i] == '\n') {
			cur_line++;
		}
	}

	return -1;
}

// Removes leading and trailing whitespace from s.
// s needs to be an allocated buffer.
	void
trim_space(char* s)
{
	char* duplicate = strdup(s);

	memset(s, 0, strlen(s));

	bool at_start = true;

	// Rebuild s without leading whitespace.
	for (char* p = duplicate; *p != '\0'; p++) {
		if (at_start && isspace((int)*p))
			continue;

		at_start = false;

		s[strlen(s)] = *p;
	}

	// Remove trailing whitespace.
	for (int i = strlen(s)-1; isspace((int)s[i]); i--) {
		s[i] = '\0';
	}

	free(duplicate);
}
