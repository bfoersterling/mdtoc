#include "unit_testing.h"
// For testing static and nonstatic functions include the .c file
// and do not compile/link the .c file of you real program.
#include "../src/heading.c"

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

START_TEST (test_atx_string)
{
	// 1
	char* output_1 = atx_string("Building From Source", 4);

	ck_assert_str_eq(output_1, "#### Building From Source");

	free(output_1);
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

	char* pretty_levels_1 = pretty_heading_levels(head_1->levels);

	ck_assert_str_eq(pretty_levels_1, "0.");

	free(pretty_levels_1);

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

	// 5 - heading that contains a link
	const char* source_5 =
		"## [my link](https://example.com)\n"
		"## foobar\n";

	const char* expected_5 =
		"1. my link (1)\n"
		"2. foobar (2)\n";

	struct heading* head_5 = parse_headings(source_5);

	size_t buffer_size_5 = 64;
	char* buffer_5 = malloc(buffer_size_5);
	memset(buffer_5, 0, buffer_size_5);
	FILE* stream_5 = fmemopen(buffer_5, buffer_size_5, "w");

	print_heading_tree(head_5, 0, stream_5);

	ck_assert_str_eq(buffer_5, expected_5);

	free(buffer_5);
	fclose(stream_5);
	free_heading_tree(head_5);

	// 6 - heading with nested content
	const char* source_6 =
		"## [link with nested **strong *and emph* content**]"
		"(https://example.com)\n"
		"## normal heading\n";

	const char* expected_6 =
		"1. link with nested strong and emph content (1)\n"
		"2. normal heading (2)\n";

	struct heading* head_6 = parse_headings(source_6);

	size_t buffer_size_6 = 128;
	char* buffer_6 = malloc(buffer_size_6);
	memset(buffer_6, 0, buffer_size_6);
	FILE* stream_6 = fmemopen(buffer_6, buffer_size_6, "w");

	print_heading_tree(head_6, 0, stream_6);

	ck_assert_str_eq(buffer_6, expected_6);

	free(buffer_6);
	fclose(stream_6);
	free_heading_tree(head_6);
}

START_TEST (test_pretty_heading_levels)
{
	int hlevels[6] = {0,2,0,1,0,0};

	char* pretty_numbering = pretty_heading_levels(hlevels);

	ck_assert_str_eq(pretty_numbering, "2.1.");

	free(pretty_numbering);
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

	TCase* tc_atx_string = tcase_create("test_atx_string");
	TCase* tc_find_heading_by_numbering = tcase_create("test_find_heading_by_numbering");
	TCase* tc_parse_headings = tcase_create("test_parse_headings");
	TCase* tc_pretty_heading_levels = tcase_create("test_pretty_heading_levels");
	TCase* tc_update_heading_levels = tcase_create("test_update_heading_levels");

	tcase_add_test(tc_atx_string, test_atx_string);
	tcase_add_test(tc_find_heading_by_numbering, test_find_heading_by_numbering);
	tcase_add_test(tc_parse_headings, test_parse_headings);
	tcase_add_test(tc_pretty_heading_levels, test_pretty_heading_levels);
	tcase_add_test(tc_update_heading_levels, test_update_heading_levels);

	suite_add_tcase(s, tc_atx_string);
	suite_add_tcase(s, tc_find_heading_by_numbering);
	suite_add_tcase(s, tc_parse_headings);
	suite_add_tcase(s, tc_pretty_heading_levels);
	suite_add_tcase(s, tc_update_heading_levels);

	return s;
}
