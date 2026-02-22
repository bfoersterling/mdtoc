#include "unit_testing.h"
// For testing static and nonstatic functions include the .c file
// and do not compile/link the .c file of you real program.
#include "../parser.c"

#include <ctype.h>

[[maybe_unused]] static void pretty_string_diff(const char* s1, const char* s2);

	static void
pretty_string_diff(const char* s1, const char* s2)
{
	printf("strlen(s1): %ld\n", strlen(s1));
	printf("strlen(s2): %ld\n", strlen(s2));

	char* longer_string = NULL;

	// Determine the shorter string.
	size_t compare_length = strlen(s1);

	if (strlen(s2) < strlen(s1)) {
		compare_length = strlen(s2);
		longer_string = (char*)s1;
	} else if (strlen(s2) > strlen(s1)) {
		longer_string = (char*)s2;
	}

	for (int i = 0; i < compare_length; i++) {
		if (s1[i] == s2[i]) {
			printf("Character %d (%c) is in both strings.\n", s1[i],
					isprint(s1[i]) ? s1[i] : 0);
		} else {
			printf("Diff: s1: >%d< (%c) - s2: >%d< (%c)\n", s1[i],
					isprint(s1[i]) ? s1[i] : 0, s2[i],
					isprint(s2[i]) ? s2[i] : 0);
		}
	}

	if (longer_string == NULL)
		return;

	printf("\n");
	if (longer_string == s1) {
		printf("s1 has the following additional characters:\n");
	} else {
		printf("s2 has the following additional characters:\n");
	}

	for (char* p = longer_string+compare_length; *p != '\0'; p++) {
		printf("%d (%c)\n", *p, isprint(*p) ? *p : 0);
	}

	printf("\n");
}

START_TEST (test_find_heading_by_numbering)
{
	// 1
	const char* source_1 =
		"## foo\n"
		"#### subfoo\n"
		"## bar\n"
		"#### subbar\n";

	struct heading* root_1 = parse_headings(source_1);

	struct heading* node_1 = NULL;

	find_heading_by_numbering(root_1, "2.1.", &node_1);

	ck_assert(node_1 != NULL);
	ck_assert_str_eq(node_1->text, "subbar");

	struct heading* node_2 = NULL;

	find_heading_by_numbering(root_1, "4.4.", &node_2);

	ck_assert(node_2 == NULL);

	struct heading* node_3 = NULL;

	find_heading_by_numbering(root_1, "2.", &node_3);

	ck_assert(node_3 != NULL);
	ck_assert_str_eq(node_3->text, "bar");

	free_heading_tree(root_1);
}

START_TEST (test_parse_headings)
{
	// 1
	const char* source_1 =
		"## first heading\n"
		"Some text.\n"
		"## second heading\n"
		"More text.\n";

	struct heading* head_1 = parse_headings(source_1);

	ck_assert(head_1 != NULL);
	ck_assert_int_eq(head_1->level, 0);
	ck_assert(head_1->next == NULL);
	ck_assert(head_1->first_child != NULL);
	ck_assert(head_1->parent == NULL);
	ck_assert_str_eq(head_1->first_child->text, "first heading");
	ck_assert_str_eq(head_1->first_child->next->text, "second heading");
	ck_assert(head_1->first_child->parent == head_1);
	ck_assert(head_1->first_child->next->parent == head_1);

	free_heading_tree(head_1);

	// 2
	const char* source_2 =
		"## foo\n"
		"#### subfoo\n"
		"## bar\n"
		"#### subbar\n";

	const char* expected_2 =
		"1. foo (1)\n"
		"  1.1. subfoo (2)\n"
		"2. bar (3)\n"
		"  2.1. subbar (4)\n";

	struct heading* head_2 = parse_headings(source_2);

	size_t buffer_size_2 = 2048;
	char* buffer_2 = malloc(buffer_size_2);
	memset(buffer_2, 0, buffer_size_2);
	FILE* stream_2 = fmemopen(buffer_2, buffer_size_2, "w");

	print_heading_tree(head_2, 0, stream_2);

	ck_assert_str_eq(buffer_2, expected_2);

	free(buffer_2);
	fclose(stream_2);
	free_heading_tree(head_2);

	// 3 - preamble
	const char* source_3 =
		"> Start with a quote.\n"
		"## foo\n"
		"Foo.\n";

	const char* expected_3 =
		"0. preamble (1)\n"
		"1. foo (2)\n";

	struct heading* head_3 = parse_headings(source_3);

	ck_assert(head_3 != NULL);
	ck_assert(head_3->first_child != NULL);
	ck_assert_str_eq(head_3->first_child->text, "preamble");
	ck_assert_int_eq(head_3->first_child->level, 2);
	ck_assert(head_3->first_child->next != NULL);
	ck_assert_str_eq(head_3->first_child->next->text, "foo");

	size_t buffer_size_3 = strlen(expected_3) * 2;
	char* buffer_3 = malloc(buffer_size_3);
	memset(buffer_3, 0, buffer_size_3);
	FILE* stream_3 = fmemopen(buffer_3, buffer_size_3, "w");

	print_heading_tree(head_3, 0, stream_3);

	ck_assert_str_eq(buffer_3, expected_3);

	free(buffer_3);
	fclose(stream_3);
	free_heading_tree(head_3);

	// 4 - empty file should not have a toc
	const char* source_4 =
		"";

	const char* expected_4 =
		"";

	struct heading* head_4 = parse_headings(source_4);

	size_t buffer_size_4 = 64;
	char* buffer_4 = malloc(buffer_size_4);
	memset(buffer_4, 0, buffer_size_4);
	FILE* stream_4 = fmemopen(buffer_4, buffer_size_4, "w");

	print_heading_tree(head_4, 0, stream_4);

	ck_assert_str_eq(buffer_4, expected_4);

	free(buffer_4);
	fclose(stream_4);
	free_heading_tree(head_4);
}

START_TEST (test_pretty_heading_levels)
{
	int hlevels[6] = {0,2,0,1,0,0};

	char* pretty_numbering = pretty_heading_levels(hlevels);

	ck_assert_str_eq(pretty_numbering, "2.1.");

	free(pretty_numbering);
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

START_TEST (test_update_heading_levels)
{
	// 1
	int current_level = 1;
	int heading_levels[6] = {0,0,0,0,0,0};
	update_heading_levels(current_level, heading_levels);
	ck_assert_int_eq(heading_levels[0], 1);
	ck_assert_int_eq(heading_levels[1], 0);
	ck_assert_int_eq(heading_levels[2], 0);
	ck_assert_int_eq(heading_levels[3], 0);
	ck_assert_int_eq(heading_levels[4], 0);
	ck_assert_int_eq(heading_levels[5], 0);

	// 2
	current_level = 4;
	update_heading_levels(current_level, heading_levels);
	ck_assert_int_eq(heading_levels[0], 1);
	ck_assert_int_eq(heading_levels[1], 0);
	ck_assert_int_eq(heading_levels[2], 0);
	ck_assert_int_eq(heading_levels[3], 1);
	ck_assert_int_eq(heading_levels[4], 0);
	ck_assert_int_eq(heading_levels[5], 0);

	// 3
	current_level = 2;
	update_heading_levels(current_level, heading_levels);
	ck_assert_int_eq(heading_levels[0], 1);
	ck_assert_int_eq(heading_levels[1], 1);
	ck_assert_int_eq(heading_levels[2], 0);
	ck_assert_int_eq(heading_levels[3], 0);
	ck_assert_int_eq(heading_levels[4], 0);
	ck_assert_int_eq(heading_levels[5], 0);
}

Suite*
parser_suite(void) {
	Suite* s = suite_create("parser");

	TCase* tc_find_heading_by_numbering = tcase_create("test_find_heading_by_numbering");
	TCase* tc_parse_headings = tcase_create("test_parse_headings");
	TCase* tc_pretty_heading_levels = tcase_create("test_pretty_heading_levels");
	TCase* tc_print_colored_markdown = tcase_create("test_print_colored_markdown");
	TCase* tc_update_heading_levels = tcase_create("test_update_heading_levels");

	tcase_add_test(tc_find_heading_by_numbering, test_find_heading_by_numbering);
	tcase_add_test(tc_parse_headings, test_parse_headings);
	tcase_add_test(tc_pretty_heading_levels, test_pretty_heading_levels);
	tcase_add_test(tc_print_colored_markdown, test_print_colored_markdown);
	tcase_add_test(tc_update_heading_levels, test_update_heading_levels);

	suite_add_tcase(s, tc_find_heading_by_numbering);
	suite_add_tcase(s, tc_parse_headings);
	suite_add_tcase(s, tc_pretty_heading_levels);
	suite_add_tcase(s, tc_print_colored_markdown);
	suite_add_tcase(s, tc_update_heading_levels);

	return s;
}
