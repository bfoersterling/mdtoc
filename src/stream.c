#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/*
 * Exit the program if "entry_path" is not a regular file or a symlink
 * that points to a regular file.
 * Error messages will be printed to stderr.
 */
	void
assert_regular_file(const char* entry_path)
{
	struct stat stat_buffer = {};
	int stat_return = stat(entry_path, &stat_buffer);

	if (stat_return != 0) {
		fprintf(stderr, "Failed to stat \"%s\": %s.\n", entry_path,
				strerror(errno));
		exit(1);
	}

	if (!S_ISREG(stat_buffer.st_mode)) {
		fprintf(stderr, "Error: \"%s\" is not a regular file.\n", entry_path);
		exit(1);
	}
}

// Return the number of lines in file "stream".
	int
count_lines(FILE* stream)
{
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

/*
 * Warning: Do NOT use this on dirs. Only pass regular files.
 * Directories will return very large values.
 */
	long
file_size(FILE* stream)
{
	assert(stream != NULL);

	long end_pos = 0;
	long initial_pos = ftell(stream);

	fseek(stream, 0, SEEK_END);

	end_pos = ftell(stream);

	fseek(stream, initial_pos, SEEK_SET);

	return end_pos;
}

// UNUSED
// Read from stdin.
// Caller has to free the returned buffer.
	char*
get_user_input(void)
{
	size_t buffer_size = 32768;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);
	char read_char = 0;

	while ((read_char = fgetc(stdin)) != EOF) {
		strcat(buffer, &read_char);
	}

	return buffer;
}

	bool
is_regular_file(const char* entry_path)
{
	struct stat stat_buffer = {};
	int stat_return = stat(entry_path, &stat_buffer);

	if (stat_return != 0)
		return false;

	if (S_ISREG(stat_buffer.st_mode))
		return true;

	return false;
}

// Returns the file position of the end of the line "line"
// in file "source_file".
// Returns -1 if the line was not found.
	long
line_end_pos(FILE* source_file, int line)
{
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
line_start_pos(FILE* source_file, int line)
{
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

/*
 * Read the file at path "file_path".
 * If "exit_on_error" is true -> exit the entire program if an error occurs.
 * Error messages go to stderr.
 * Return value: Content of "file_path" or NULL if it is not a regular file.
 * Caller has to free the returned buffer.
 */
	char*
read_file(const char* file_path, bool exit_on_error)
{
	char* buffer = NULL;

	FILE* f = fopen(file_path, "r");

	if (f == NULL) {
		fprintf(stderr, "Error opening \"%s\": %s.\n", file_path, strerror(errno));
		if (exit_on_error)
			exit(1);
		goto end;
	}

	if (!is_regular_file(file_path)) {
		fprintf(stderr, "Error: \"%s\" is not a regular file.\n", file_path);
		if (exit_on_error) {
			exit(1);
		}
		goto end;
	}

	size_t f_size = (size_t)file_size(f);

	size_t buffer_size = f_size + 1;
	buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	fread(buffer, 1, f_size, f);

	if (ferror(f) != 0) {
		fprintf(stderr, "Error reading file %s.\n", file_path);
		if (exit_on_error)
			exit(1);
	}

end:
	fclose(f);

	return buffer;
}

// Read only a part of a file, starting from byte "from" until byte "to".
// Caller has to free the returned buffer.
	char*
read_file_section(FILE* source_file, long from, long to)
{
	if (to == 0)
		return NULL;

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

/*
 * Reads "stream" into a buffer and returns it.
 * "stream" should NEVER be a real file as the file size is not determined.
 * "stream" should be either stdin or a mem stream for unit testing.
 * The size of the buffer is an arbitrarily chosen constant value.
 * (Large streams will cause overflows.)
 * Caller has to free the returned buffer.
 */
	char*
read_stream(FILE* stream, bool exit_on_error)
{
	assert(stream != NULL);

	// Arbitrary buffer size that should handle most user inputs and piped
	// input.
	size_t buffer_size = 32768;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	fread(buffer, 1, buffer_size, stream);

	if (ferror(stream) != 0) {
		fprintf(stderr, "Error reading stream %d.\n", fileno(stream));
		if (exit_on_error)
			exit(1);
	}

	return buffer;
}
