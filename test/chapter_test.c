#include "unit_testing.h"

#include "../chapter.c"

#include <unistd.h>
#include <errno.h>


START_TEST (test_chapter_end_pos)
{
	char* source_1 =
		"## foo\n"
		"Some text\n"
		"#### subfoo\n"
		"## bar\n"
		"#### subbar";

	FILE* stream_1 = fmemopen(source_1, strlen(source_1), "r");

	struct heading* head = parse_headings(source_1);

	struct heading* foo = head->first_child;
	struct heading* subfoo = head->first_child->first_child;
	struct heading* bar = head->first_child->next;

	int end_pos_1 = chapter_end_pos(stream_1, foo);
	int end_pos_2 = chapter_end_pos(stream_1, bar);
	int end_pos_3 = chapter_end_pos(stream_1, subfoo);

	ck_assert_int_eq(end_pos_1, 29);
	ck_assert_int_eq(end_pos_2, strlen(source_1));
	ck_assert_int_eq(end_pos_3, 29);

	free_heading_tree(head);
	fclose(stream_1);
}

START_TEST (test_fetch_chapter)
{
	const char* file_content_1 =
		"## foo\n"
		"#### subfoo\n"
		"Paragraph under subfoo\n"
		"## bar\n"
		"#### subbar\n"
		"Paragraph under subbar\n";

	FILE* stream_1 = fmemopen((char*)file_content_1, strlen(file_content_1), "r");

	// 1 - chapter exists - no trailing dot
	char* output_1 = fetch_chapter(stream_1, "1.1");

	ck_assert_str_eq(output_1, "#### subfoo\nParagraph under subfoo\n");

	free(output_1);

	// 2 - chapter is NULL
	char* output_2 = fetch_chapter(stream_1, NULL);

	ck_assert(output_2 == NULL);

	free(output_2);

	// 3 - chapter does not exist
	char* output_3 = fetch_chapter(stream_1, "3.3");

	ck_assert(output_3 == NULL);

	free(output_3);

	// 4 - chapter exists - trailing dot
	char* output_4 = fetch_chapter(stream_1, "2.1.");

	ck_assert_str_eq(output_4, "#### subbar\nParagraph under subbar\n");

	free(output_4);

	fclose(stream_1);
}

START_TEST (test_print_chapter_no_color)
{
	const char* source_1 =
		"## foo\n"
		"#### subfoo\n"
		"Some subfoo text.\n"
		"## bar\n"
		"#### subbar\n"
		"Some subbar text.\n";
	FILE* input_stream_1 = fmemopen((char*)source_1, strlen(source_1), "r");

	size_t output_size_1 = 4096;
	char* output_1 = malloc(output_size_1);
	memset(output_1, 0, output_size_1);
	FILE* output_stream_1 = fmemopen(output_1, output_size_1, "w");

	// 1
	const char* expected_1 =
		"## foo\n"
		"#### subfoo\n"
		"Some subfoo text.\n";

	print_chapter_no_color(input_stream_1, "1.", output_stream_1);

	ck_assert_str_eq(output_1, expected_1);

	free(output_1);
	fclose(output_stream_1);

	// 2
	size_t output_size_2 = 4096;
	char* output_2 = malloc(output_size_2);
	memset(output_2, 0, output_size_2);
	FILE* output_stream_2 = fmemopen(output_2, output_size_2, "w");

	const char* expected_2 =
		"## bar\n"
		"#### subbar\n"
		"Some subbar text.\n";

	print_chapter_no_color(input_stream_1, "2.", output_stream_2);

	ck_assert_str_eq(output_2, expected_2);

	free(output_2);
	fclose(output_stream_2);

	// 3
	size_t output_size_3 = 4096;
	char* output_3 = malloc(output_size_3);
	memset(output_3, 0, output_size_3);
	FILE* output_stream_3 = fmemopen(output_3, output_size_3, "w");

	const char* expected_3 =
		"#### subfoo\n"
		"Some subfoo text.\n";

	print_chapter_no_color(input_stream_1, "1.1.", output_stream_3);

	ck_assert_str_eq(output_3, expected_3);

	free(output_3);
	fclose(output_stream_3);

	// 4
	size_t output_size_4 = 4096;
	char* output_4 = malloc(output_size_4);
	memset(output_4, 0, output_size_4);
	FILE* output_stream_4 = fmemopen(output_4, output_size_4, "w");

	const char* expected_4 =
		"#### subbar\n"
		"Some subbar text.\n";

	print_chapter_no_color(input_stream_1, "2.1.", output_stream_4);

	ck_assert_str_eq(output_4, expected_4);

	free(output_4);
	fclose(output_stream_4);

	fclose(input_stream_1);
}

	Suite*
chapter_suite(void)
{
	Suite* s = suite_create("chapter");

	TCase* tc_chapter_end_pos = tcase_create("test_chapter_end_pos");
	TCase* tc_fetch_chapter = tcase_create("test_fetch_chapter");
	TCase* tc_print_chapter_no_color = tcase_create("test_print_chapter_no_color");

	tcase_add_test(tc_chapter_end_pos, test_chapter_end_pos);
	tcase_add_test(tc_fetch_chapter, test_fetch_chapter);
	tcase_add_test(tc_print_chapter_no_color, test_print_chapter_no_color);

	suite_add_tcase(s, tc_chapter_end_pos);
	suite_add_tcase(s, tc_fetch_chapter);
	suite_add_tcase(s, tc_print_chapter_no_color);

	return s;
}
