#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return the number of lines in file "stream".
int
count_lines(FILE* stream) {
	assert(stream != NULL);

	long initial_file_pos = ftell(stream);
	int line_count = 0;
	size_t line_buffer_size = LINE_MAX;
	char* line_buffer = malloc(LINE_MAX);
	memset(line_buffer, 0, LINE_MAX);

	fseek(stream, 0, SEEK_SET);

	while(fgets(line_buffer, line_buffer_size, stream) != NULL) {
		line_count++;
	}

	fseek(stream, initial_file_pos, SEEK_SET);
	free(line_buffer);

	return line_count;
}

long
file_size(FILE* stream) {
	assert(stream != NULL);

	long end_pos = 0;
	long initial_pos = ftell(stream);

	fseek(stream, 0, SEEK_END);

	end_pos = ftell(stream);

	fseek(stream, initial_pos, SEEK_SET);

	return end_pos;
}

// Read from stdin.
// Caller has to free the returned buffer.
char*
get_user_input(void) {
	size_t buffer_size = 32768;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);
	char read_char = 0;

	while (read_char != EOF) {
		read_char = fgetc(stdin);
		strcat(buffer, &read_char);
	}

	return buffer;
}

// Returns the file position of the end of the line "line"
// in file "source_file".
// Returns -1 if the line was not found.
long
line_end_pos(FILE* source_file, int line) {
	assert(line > 0);
	assert(source_file != NULL);

	long initial_file_pos = ftell(source_file);
	int current_line = 1;
	long end_pos = 0;
	size_t line_buffer_size = LINE_MAX;
	char* line_buffer = malloc(line_buffer_size);
	memset(line_buffer, 0, line_buffer_size);

	fseek(source_file, 0, SEEK_SET);

	while(fgets(line_buffer, line_buffer_size, source_file) != NULL) {
		if (current_line == line) {
			free(line_buffer);
			end_pos = ftell(source_file);
			fseek(source_file, initial_file_pos, SEEK_SET);
			return end_pos;
		}
		current_line++;
	}

	free(line_buffer);
	fseek(source_file, initial_file_pos, SEEK_SET);
	return -1;
}

// Returns the file position of the start of the line "line"
// in file "source_file".
// Returns -1 if the line was not found.
long
line_start_pos(FILE* source_file, int line) {
	assert(line > 0);
	assert(source_file != NULL);

	long initial_file_pos = ftell(source_file);
	int current_line = 1;
	long start_pos = 0;
	size_t line_buffer_size = LINE_MAX;
	char* line_buffer = malloc(line_buffer_size);
	memset(line_buffer, 0, line_buffer_size);

	fseek(source_file, 0, SEEK_SET);

	do {
		if (current_line == line) {
			free(line_buffer);
			start_pos = ftell(source_file);
			fseek(source_file, initial_file_pos, SEEK_SET);
			return start_pos;
		}
		current_line++;
	} while(fgets(line_buffer, line_buffer_size, source_file) != NULL);

	free(line_buffer);
	fseek(source_file, initial_file_pos, SEEK_SET);
	return -1;
}

// Caller has to free the returned buffer.
// Error messages go to stderr.
char*
read_file(const char* file_path) {
	FILE* f = fopen(file_path, "r");

	if (f == NULL) {
		fprintf(stderr, "Error opening %s: %s.\n", file_path, strerror(errno));
		exit(1);
	}

	size_t f_size = (size_t)file_size(f);

	size_t buffer_size = f_size + 1;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	fread(buffer, 1, f_size, f);

	fclose(f);

	return buffer;
}

// Read only a part of a file, starting from byte "from" until byte "to".
// Caller has to free the returned buffer.
char*
read_file_section(FILE* source_file, long from, long to) {
	assert(from < to);

	long initial_file_pos = ftell(source_file);
	size_t buffer_size = to - from + 1;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	fseek(source_file, from, SEEK_SET);

	fread(buffer, 1, to-from, source_file);

	fseek(source_file, initial_file_pos, SEEK_SET);

	return buffer;
}
