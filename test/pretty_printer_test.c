#include "unit_testing.h"

#include <stdlib.h>

#include "../src/pretty_printer.c"

START_TEST (test_pretty_print_heading_levels)
{
	int hlevels[6] = {0,2,0,2,0,0};
	char* expected = "2.2.";

	size_t output_size = 32;
	char* output = malloc(output_size);
	memset(output, 0, output_size);
	FILE* stream = fmemopen(output, output_size, "w");

	pretty_print_heading_levels(hlevels, stream);

	ck_assert_str_eq(output, expected);

	fclose(stream);
	free(output);
}

START_TEST (test_print_colored_markdown)
{
	// 1 - heading, subheading, paragraph
	const char* input_1 =
		"## foo\n"
		"#### subfoo\n"
		"Some text.\n";

	const char* expected_1 =
		"\033[32m## foo\n"
		"\n"
		"\033[32m#### subfoo\n"
		"\n"
		"\033[0m"
		"Some text.\n";

	// Make sure we enough space for input_1 and all additional escape codes.
	size_t buffer_size_1 = strlen(input_1) * 8;
	char* buffer_1 = malloc(buffer_size_1);
	memset(buffer_1, 0, buffer_size_1);
	FILE* stream_1 = fmemopen(buffer_1, buffer_size_1, "w");

	print_colored_markdown(input_1, stream_1);

	ck_assert_mem_eq(buffer_1, expected_1, strlen(expected_1));

	free(buffer_1);
	fclose(stream_1);

	// 2 - paragraph with two lines
	const char* input_2 =
		"## foo\n"
		"#### subfoo\n"
		"Some text.\n"
		"Some more text.\n";

	const char* expected_2 =
		"\033[32m## foo\n"
		"\n"
		"\033[32m#### subfoo\n"
		"\n"
		"\033[0m"
		"Some text. "
		"Some more text.\n";

	size_t buffer_size_2 = strlen(input_2) * 8;
	char* buffer_2 = malloc(buffer_size_2);
	memset(buffer_2, 0, buffer_size_2);
	FILE* stream_2 = fmemopen(buffer_2, buffer_size_2, "w");

	print_colored_markdown(input_2, stream_2);

	ck_assert_mem_eq(buffer_2, expected_2, strlen(expected_2));

	free(buffer_2);
	fclose(stream_2);

	// 3 - code block
	const char* input_3 =
		"## show code\n"
		"```\n"
		"cat foo.txt\n"
		"ls bar\n"
		"```\n";

	const char* expected_3 =
		"\033[32m## show code\n"
		"\n"
		"\033[33;100mcat foo.txt\033[0m\033[K\n"
		"\033[33;100mls bar\033[0m\033[K\n"
		"\033[0m";

	size_t buffer_size_3 = strlen(input_3) * 8;
	char* buffer_3 = malloc(buffer_size_3);
	memset(buffer_3, 0, buffer_size_3);
	FILE* stream_3 = fmemopen(buffer_3, buffer_size_3, "w");

	print_colored_markdown(input_3, stream_3);

	ck_assert_str_eq(buffer_3, expected_3);

	free(buffer_3);
	fclose(stream_3);

	// 4 - bullet list
	const char* input_4 =
		"## foo\n"
		"\n"
		"- item1\n"
		"- item2\n"
		"- item3\n";

	const char* expected_4 =
		"\033[32m## foo\n"
		"\n"
		"\033[0m"
		"- \033[0mitem1\n"
		"- \033[0mitem2\n"
		"- \033[0mitem3\n"
		"\033[0m";

	size_t buffer_size_4 = strlen(input_4) * 8;
	char* buffer_4 = malloc(buffer_size_4);
	memset(buffer_4, 0, buffer_size_4);
	FILE* stream_4 = fmemopen(buffer_4, buffer_size_4, "w");

	print_colored_markdown(input_4, stream_4);

	ck_assert_str_eq(buffer_4, expected_4);

	free(buffer_4);
	fclose(stream_4);

	// 5 - ordered list
	const char* input_5 =
		"## foo\n"
		"1. item1\n"
		"1. item2\n"
		"1. item3\n";

	const char* expected_5 =
		"\033[32m## foo\n"
		"\n"
		"\033[0m"
		"1. \033[0mitem1\n"
		"2. \033[0mitem2\n"
		"3. \033[0mitem3\n"
		"\033[0m";

	size_t buffer_size_5 = strlen(input_5) * 8;
	char* buffer_5 = malloc(buffer_size_5);
	memset(buffer_5, 0, buffer_size_5);
	FILE* stream_5 = fmemopen(buffer_5, buffer_size_5, "w");

	print_colored_markdown(input_5, stream_5);

	ck_assert_str_eq(buffer_5, expected_5);

	free(buffer_5);
	fclose(stream_5);

	// 6 - thematic break
	const char* input_6 =
		"## foo\n"
		"Foo.\n"
		"\n"
		"---\n"
		"## bar\n"
		"Bar.\n"
		"\n"
		"---\n";

	const char* expected_6 =
		"\033[32m## foo\n"
		"\n"
		"\033[0m"
		"Foo.\n"
		"\n"
		"\033[34;100m----------------------------------------------------------------"
		"-----------------"
		"\033[0m\n"
		"\n"
		"\033[32m## bar\n"
		"\n"
		"\033[0m"
		"Bar.\n"
		"\n"
		"\033[34;100m----------------------------------------------------------------"
		"-----------------"
		"\033[0m\n"
		"\033[0m";

	size_t buffer_size_6 = strlen(input_6) * 8;
	char* buffer_6 = malloc(buffer_size_6);
	memset(buffer_6, 0, buffer_size_6);
	FILE* stream_6 = fmemopen(buffer_6, buffer_size_6, "w");

	print_colored_markdown(input_6, stream_6);

	ck_assert_str_eq(buffer_6, expected_6);

	free(buffer_6);
	fclose(stream_6);

	// 7 - strong
	const char* input_7 =
		"## foo\n"
		"This is **really** strong.\n";
	const char* expected_7 =
		"\033[32m## foo\n"
		"\n"
		"\033[0m"
		"This is \033[1mreally\033[0m strong.\n"
		"\033[0m";

	size_t buffer_size_7 = strlen(expected_7) * 2;
	char* buffer_7 = malloc(buffer_size_7);
	memset(buffer_7, 0, buffer_size_7);
	FILE* stream_7 = fmemopen(buffer_7, buffer_size_7, "w");

	print_colored_markdown(input_7, stream_7);

	ck_assert_str_eq(buffer_7, expected_7);

	free(buffer_7);
	fclose(stream_7);

	// 8 - block quote with inline code
	const char* input_8 =
		"> A non-xterm extension is the hyperlink, `ESC ]8;;link ST` from 2017,\\\n"
		"used by VTE, iTerm2, and mintty, among others.";
	const char* expected_8 =
		"\033[35mA non-xterm extension is the hyperlink, "
		"\033[36mESC ]8;;link ST\033[0m\033[35m from 2017,"
		"\n"
		"\033[35mused by VTE, iTerm2, and mintty, among others."
		"\n"
		"\033[0m";

	size_t buffer_size_8 = strlen(expected_8) * 2;
	char* buffer_8 = malloc(buffer_size_8);
	memset(buffer_8, 0, buffer_size_8);
	FILE* stream_8 = fmemopen(buffer_8, buffer_size_8, "w");

	print_colored_markdown(input_8, stream_8);

	ck_assert_str_eq(buffer_8, expected_8);

	free(buffer_8);
	fclose(stream_8);

	// 9 - emph
	const char* input_9 =
		"This is *emphasized* and the rest is normal.\n";

	const char* expected_9 =
		"\033[0mThis is emphasized\033[0m and the rest is normal.\n\033[0m";

	size_t buffer_size_9 = strlen(expected_9) * 2;
	char* buffer_9 = malloc(buffer_size_9);
	memset(buffer_9, 0, buffer_size_9);
	FILE* stream_9 = fmemopen(buffer_9, buffer_size_9, "w");

	print_colored_markdown(input_9, stream_9);

	ck_assert_str_eq(buffer_9, expected_9);

	free(buffer_9);
	fclose(stream_9);
}

Suite*
pretty_printing_suite(void)
{
	Suite* s = suite_create("pretty_printing");

	TCase* tc_pretty_print_heading_levels = tcase_create("test_pretty_print_heading_levels");
	TCase* tc_print_colored_markdown = tcase_create("test_print_colored_markdown");

	tcase_add_test(tc_pretty_print_heading_levels, test_pretty_print_heading_levels);
	tcase_add_test(tc_print_colored_markdown, test_print_colored_markdown);

	suite_add_tcase(s, tc_pretty_print_heading_levels);
	suite_add_tcase(s, tc_print_colored_markdown);

	return s;
}
