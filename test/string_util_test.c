#include "unit_testing.h"

#include "../string_util.c"

#include <string.h>
#include <stdlib.h>

START_TEST (test_ensure_trailing_dot)
{
	// 1
	size_t buffer_size_1 = 4;
	char* buffer_1 = malloc(buffer_size_1);
	memset(buffer_1, 0, buffer_size_1);
	strcpy(buffer_1, "foo");

	ensure_trailing_dot(&buffer_1);

	ck_assert_str_eq(buffer_1, "foo.");

	free(buffer_1);

	// 2 - should not crash
	ensure_trailing_dot(NULL);
	char* buffer_2 = NULL;
	ensure_trailing_dot(&buffer_2);

	// 3 - trailing dot already present
	size_t buffer_size_3 = 5;
	char* buffer_3 = malloc(buffer_size_3);
	memset(buffer_3, 0, buffer_size_3);
	strcpy(buffer_3, "foo.");

	ensure_trailing_dot(&buffer_3);

	ck_assert_str_eq(buffer_3, "foo.");

	free(buffer_3);

	// 4 - allocated empty buffer -> should receive a trailing dot
	size_t buffer_size_4 = 8;
	char* buffer_4 = malloc(buffer_size_4);
	memset(buffer_4, 0, buffer_size_4);

	ensure_trailing_dot(&buffer_4);

	ck_assert_str_eq(buffer_4, ".");

	free(buffer_4);
}

START_TEST (test_first_digit_in_str)
{
	// 1 - string with digits
	const char* output_1 = first_digit_in_str("foo99");
	ck_assert_str_eq(output_1, "99");

	// 2 - NULL
	const char* output_2 = first_digit_in_str(NULL);
	ck_assert(output_2 == NULL);

	// 3 - empty string
	const char* output_3 = first_digit_in_str("");
	ck_assert(output_3 == NULL);

	// 4 - string without digit
	const char* output_4 = first_digit_in_str("Foobar.");
	ck_assert(output_4 == NULL);

	// 5 - string with multiple digits separated by nondigits
	const char* output_5 = first_digit_in_str("foo 12 bar 34.");
	ck_assert_str_eq(output_5, "12 bar 34.");

	// 6 - string starts with digit
	const char* output_6 = first_digit_in_str("1foobar2");
	ck_assert_str_eq(output_6, "1foobar2");
}

START_TEST (test_numbering_with_trailing_dot)
{
	// 1
	char* output_1 = numbering_with_trailing_dot("foo");

	ck_assert_str_eq(output_1, "foo.");

	free(output_1);

	// 2
	char* output_2 = numbering_with_trailing_dot("bar.");
	
	ck_assert_str_eq(output_2, "bar.");

	free(output_2);
}

START_TEST (test_merge_strings)
{
	// 1
	char* output_1 = merge_strings("foo", "bar");

	ck_assert_str_eq(output_1, "foobar");

	free(output_1);

	// 2 - s1 is empty
	char* output_2 = merge_strings("", "foo");

	ck_assert_str_eq(output_2, "foo");

	free(output_2);

	// 3 - s2 is empty
	char* output_3 = merge_strings("foo", "");

	ck_assert_str_eq(output_3, "foo");

	free(output_3);

	// 4 - s1 is NULL
	char* output_4 = merge_strings(NULL, "foo");

	ck_assert(output_4 == NULL);

	free(output_4);

	// 5 - s2 is NULL
	char* output_5 = merge_strings("foo", NULL);

	ck_assert(output_5 == NULL);

	free(output_5);

	// 6 - allocated prefix
	size_t prefix_size_6 = 256;
	char* prefix_6 = malloc(prefix_size_6);
	memset(prefix_6, 0, prefix_size_6);
	strcpy(prefix_6, "pre");

	char* output_6 = merge_strings(prefix_6, "fix");

	ck_assert_str_eq(output_6, "prefix");

	free(prefix_6);
	free(output_6);

	// 7 - allocated s and allocated prefix
	size_t prefix_size_7 = 256;
	size_t postfix_size_7 = 256;
	char* prefix_7 = malloc(prefix_size_7);
	char* postfix_7 = malloc(postfix_size_7);
	memset(prefix_7, 0, prefix_size_7);
	memset(postfix_7, 0, postfix_size_7);
	strcpy(prefix_7, "foo");
	strcpy(postfix_7, "bar");

	char* output_7 = merge_strings(prefix_7, postfix_7);

	ck_assert_str_eq(output_7, "foobar");

	free(prefix_7);
	free(postfix_7);
	free(output_7);

	// 8 - s is filled with null terminators
	size_t postfix_size_8 = 64;
	char* postfix_8 = malloc(postfix_size_8);
	memset(postfix_8, 0, postfix_size_8);

	char* output_8 = merge_strings("foo", postfix_8);

	ck_assert_str_eq(output_8, "foo");

	free(postfix_8);
	free(output_8);

	// 9 - prefix is filled with null terminators
	size_t prefix_size_9 = 64;
	char* prefix_9 = malloc(prefix_size_9);
	memset(prefix_9, 0, prefix_size_9);

	char* output_9 = merge_strings(prefix_9, "hello");

	ck_assert_str_eq(output_9, "hello");

	free(output_9);
	free(prefix_9);
}

START_TEST (test_print_by_line_column)
{
	const char* input_1 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	const char* input_2 =
		"line 1\r\n"
		"line 2\r\n"
		"line 3\r\n";

	// 1 - last line - lf
	size_t output_size_1 = 256;
	char* output_1 = malloc(output_size_1);
	memset(output_1, 0, output_size_1);
	FILE* stream_1 = fmemopen(output_1, output_size_1, "w");

	print_by_line_column(input_1, 2, 0, 2, 6, stream_1);

	ck_assert_str_eq(output_1, "line 3\n");

	free(output_1);
	fclose(stream_1);

	// 2 - first two lines - lf
	size_t output_size_2 = 256;
	char* output_2 = malloc(output_size_2);
	memset(output_2, 0, output_size_2);
	FILE* stream_2 = fmemopen(output_2, output_size_2, "w");

	print_by_line_column(input_1, 0, 0, 2, 0, stream_2);

	ck_assert_str_eq(output_2, "line 1\nline 2\n");

	free(output_2);
	fclose(stream_2);

	// 3 - first line - lf
	size_t output_size_3 = 256;
	char* output_3 = malloc(output_size_3);
	memset(output_3, 0, output_size_3);
	FILE* stream_3 = fmemopen(output_3, output_size_3, "w");

	print_by_line_column(input_1, 0, 0, 1, 0, stream_3);

	ck_assert_str_eq(output_3, "line 1\n");
	
	free(output_3);
	fclose(stream_3);

	// 4 - last line - crlf
	size_t output_size_4 = 256;
	char* output_4 = malloc(output_size_4);
	memset(output_4, 0, output_size_4);
	FILE* stream_4 = fmemopen(output_4, output_size_4, "w");

	print_by_line_column(input_2, 2, 0, 2, 6, stream_4);

	ck_assert_str_eq(output_4, "line 3\n");

	free(output_4);
	fclose(stream_4);

	// 5 - first two characters of line 2 - crlf
	size_t output_size_5 = 256;
	char* output_5 = malloc(output_size_5);
	memset(output_5, 0, output_size_5);
	FILE* stream_5 = fmemopen(output_5, output_size_5, "w");

	print_by_line_column(input_2, 1, 0, 1, 2, stream_5);

	ck_assert_str_eq(output_5, "li");

	free(output_5);
	fclose(stream_5);

	// 6 - out of range - crlf
	size_t output_size_6 = 256;
	char* output_6 = malloc(output_size_6);
	memset(output_6, 0, output_size_6);
	FILE* stream_6 = fmemopen(output_6, output_size_6, "w");

	print_by_line_column(input_2, 4, 5, 6, 5, stream_6);

	ck_assert_str_eq(output_6, "");

	free(output_6);
	fclose(stream_6);
}

START_TEST (test_str_from_int)
{
	// 1
	char* output_1 = str_from_int(1234567890);

	ck_assert_str_eq(output_1, "1234567890");

	free(output_1);

	// 2
	char* output_2 = str_from_int(0);

	ck_assert_str_eq(output_2, "0");

	free(output_2);
}

START_TEST (test_string_line_span)
{
	// 1 - Last two lines.
	const char* input_1 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	const char* expected_1 =
		"line 2\n"
		"line 3\n";

	char* output_1 = string_line_span(input_1, 2, 3);

	ck_assert_str_eq(output_1, expected_1);

	free(output_1);

	// 2 - Only second line.
	const char* input_2 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	const char* expected_2 =
		"line 2\n";

	char* output_2 = string_line_span(input_2, 2, 2);

	ck_assert_str_eq(output_2, expected_2);

	free(output_2);

	// 3 - Only the first line.
	const char* input_3 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	const char* expected_3 =
		"line 1\n";

	char* output_3 = string_line_span(input_3, 1, 1);

	ck_assert_str_eq(output_3, expected_3);

	free(output_3);

	// 4 - Only the last line.
	const char* input_4 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	const char* expected_4 =
		"line 3\n";

	char* output_4 = string_line_span(input_4, 3, 3);

	ck_assert_str_eq(output_4, expected_4);

	free(output_4);

	// 5 - All three lines.
	const char* input_5 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	const char* expected_5 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	char* output_5 = string_line_span(input_5, 1, 3);

	ck_assert_str_eq(output_5, expected_5);

	free(output_5);

	// 6 - End line number out of range.
	const char* input_6 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	char* output_6 = string_line_span(input_6, 2, 10);

	ck_assert(output_6 == NULL);

	free(output_6);

	// 7 - Everything out of range.
	const char* input_7 =
		"line 1\n"
		"line 2\n"
		"line 3\n";

	char* output_7 = string_line_span(input_7, 5, 10);

	ck_assert(output_7 == NULL);

	free(output_7);

	// 8 - Empty lines in between.
	const char* input_8 =
		"line 1\n"
		"\n"
		"line 3\n"
		"\n"
		"line 5\n";

	const char* expected_8 =
		"line 3\n"
		"\n"
		"line 5\n";

	char* output_8 = string_line_span(input_8, 3, 5);

	ck_assert_str_eq(output_8, expected_8);

	free(output_8);

	// 9 - Empty line at start of span.
	const char* input_9 =
		"line 1\n"
		"\n"
		"line 3\n";

	const char* expected_9 =
		"\n"
		"line 3\n";

	char* output_9 = string_line_span(input_9, 2, 3);

	ck_assert_str_eq(output_9, expected_9);

	free(output_9);
}

START_TEST (test_trim_space)
{
	size_t buffer_size_1 = 256;
	char* buffer_1 = malloc(buffer_size_1);
	memset(buffer_1, 0, buffer_size_1);

	// 1 - one space at the start
	strcpy(buffer_1, " foo");

	trim_space(buffer_1);
	ck_assert_str_eq(buffer_1, "foo");

	// 2 - two spaces at the start
	memset(buffer_1, 0, buffer_size_1);
	strcpy(buffer_1, "  foo");

	trim_space(buffer_1);
	ck_assert_str_eq(buffer_1, "foo");

	// 3 - one tab and one space at the start
	memset(buffer_1, 0, buffer_size_1);
	strcpy(buffer_1, "\t foo");

	trim_space(buffer_1);
	ck_assert_str_eq(buffer_1, "foo");

	// 4 - whitespace at start and in between
	memset(buffer_1, 0, buffer_size_1);
	strcpy(buffer_1, "\t foo bar");

	trim_space(buffer_1);
	ck_assert_str_eq(buffer_1, "foo bar");

	// 5 - whitespace at the end and in between
	memset(buffer_1, 0, buffer_size_1);
	strcpy(buffer_1, "foo bar\t ");

	trim_space(buffer_1);
	ck_assert_str_eq(buffer_1, "foo bar");

	// 6 - whitespace at the start, end and in between
	memset(buffer_1, 0, buffer_size_1);
	strcpy(buffer_1, "\t foo\t and\t  bar \t");

	trim_space(buffer_1);
	ck_assert_str_eq(buffer_1, "foo\t and\t  bar");

	free(buffer_1);
}

Suite*
string_util_suite(void) {
	Suite* s = suite_create("string_util");

	TCase* tc_ensure_trailing_dot = tcase_create("test_ensure_trailing_dot");
	TCase* tc_first_digit_in_str = tcase_create("test_first_digit_in_str");
	TCase* tc_numbering_with_trailing_dot = tcase_create("test_numbering_with_trailing_dot");
	TCase* tc_merge_strings = tcase_create("test_merge_strings");
	TCase* tc_print_by_line_column = tcase_create("test_print_by_line_column");
	TCase* tc_str_from_int = tcase_create("test_str_from_int");
	TCase* tc_string_line_span = tcase_create("test_string_line_span");
	TCase* tc_trim_space = tcase_create("test_trim_space");

	tcase_add_test(tc_ensure_trailing_dot, test_ensure_trailing_dot);
	tcase_add_test(tc_first_digit_in_str, test_first_digit_in_str);
	tcase_add_test(tc_numbering_with_trailing_dot, test_numbering_with_trailing_dot);
	tcase_add_test(tc_merge_strings, test_merge_strings);
	tcase_add_test(tc_print_by_line_column, test_print_by_line_column);
	tcase_add_test(tc_str_from_int, test_str_from_int);
	tcase_add_test(tc_string_line_span, test_string_line_span);
	tcase_add_test(tc_trim_space, test_trim_space);

	suite_add_tcase(s, tc_ensure_trailing_dot);
	suite_add_tcase(s, tc_first_digit_in_str);
	suite_add_tcase(s, tc_numbering_with_trailing_dot);
	suite_add_tcase(s, tc_merge_strings);
	suite_add_tcase(s, tc_print_by_line_column);
	suite_add_tcase(s, tc_str_from_int);
	suite_add_tcase(s, tc_string_line_span);
	suite_add_tcase(s, tc_trim_space);

	return s;
}
