#include "unit_testing.h"

#include "../src/chapter.c"

#include <unistd.h>
#include <errno.h>

START_TEST (test_find_chapter_by_numbering)
{
	// 1
	const char* source_1 =
		"## foo\n"
		"#### subfoo\n"
		"## bar\n"
		"#### subbar\n"
		"Subbar text.\n";

	struct chapter* root_chapter_1 = parse_chapters(source_1);

	struct chapter* needle_chapter_1 =
		find_chapter_by_numbering(root_chapter_1, "2.1.");

	ck_assert(needle_chapter_1 != NULL);
	ck_assert_str_eq(needle_chapter_1->body, "Subbar text.\n");

	free_chapter_and_heading_tree(root_chapter_1);

	// 2 - out of tree chapter
	const char* source_2 =
		"## foo\n"
		"Foo text.\n"
		"# out of tree\n"
		"Out of tree text.\n";

	struct chapter* root_chapter_2 = parse_chapters(source_2);

	struct chapter* needle_chapter_2 =
		find_chapter_by_numbering(root_chapter_2, "1.");

	ck_assert(needle_chapter_2 != NULL);
	ck_assert_str_eq(needle_chapter_2->body, "Foo text.\n");

	free_chapter_and_heading_tree(root_chapter_2);
}

START_TEST (test_parse_chapters)
{
	// 1
	const char* source_1 =
		"## foo\n"
		"Foo text.\n"
		"## bar\n"
		"Bar text.\n";

	struct chapter* root_chapter_1 = parse_chapters(source_1);

	ck_assert(root_chapter_1 != NULL);
	ck_assert(root_chapter_1->title != NULL);
	ck_assert(root_chapter_1->title->text != NULL);
	ck_assert_str_eq(root_chapter_1->title->text, "");
	ck_assert_int_eq(root_chapter_1->start_line, 0);
	ck_assert_int_eq(root_chapter_1->end_line, 0);
	ck_assert_str_eq(root_chapter_1->body, "");
	ck_assert(root_chapter_1->first_child->body != NULL);
	ck_assert_str_eq(root_chapter_1->first_child->body, "Foo text.\n");
	ck_assert_str_eq(root_chapter_1->first_child->title->text, "foo");
	ck_assert(root_chapter_1->first_child->next != NULL);
	ck_assert_str_eq(root_chapter_1->first_child->next->body, "Bar text.\n");
	ck_assert_str_eq(root_chapter_1->first_child->next->title->text, "bar");

	free_chapter_and_heading_tree(root_chapter_1);

	// 2 - nested headings
	const char* source_2 =
		"## foo\n"
		"#### subfoo\n"
		"Subfoo text.\n"
		"## bar\n"
		"#### subbar\n"
		"Subbar text.\n";

	struct chapter* root_chapter_2 = parse_chapters(source_2);

	ck_assert(root_chapter_2 != NULL);

	ck_assert_str_eq(root_chapter_2->first_child->first_child->body,
			"Subfoo text.\n");

	ck_assert(root_chapter_2->first_child->next->first_child != NULL);
	ck_assert(root_chapter_2->first_child->next->first_child->title != NULL);
	ck_assert_str_eq(root_chapter_2->first_child->next->first_child->title->text,
			"subbar");
	ck_assert_str_eq(root_chapter_2->first_child->next->first_child->body,
			"Subbar text.\n");

	free_chapter_and_heading_tree(root_chapter_2);
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

	size_t output_size_1 = 4096;
	char* output_1 = malloc(output_size_1);
	memset(output_1, 0, output_size_1);
	FILE* output_stream_1 = fmemopen(output_1, output_size_1, "w");

	// 1
	const char* expected_1 =
		"## foo\n"
		"#### subfoo\n"
		"Some subfoo text.\n";

	print_chapter_no_color(source_1, "1.", output_stream_1);

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

	print_chapter_no_color(source_1, "2.", output_stream_2);

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

	print_chapter_no_color(source_1, "1.1.", output_stream_3);

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

	print_chapter_no_color(source_1, "2.1.", output_stream_4);

	ck_assert_str_eq(output_4, expected_4);

	free(output_4);
	fclose(output_stream_4);
}

	Suite*
chapter_suite(void)
{
	Suite* s = suite_create("chapter");

	TCase* tc_find_chapter_by_numbering = tcase_create("test_find_chapter_by_numbering");
	TCase* tc_parse_chapters = tcase_create("test_parse_chapters");
	TCase* tc_print_chapter_no_color = tcase_create("test_print_chapter_no_color");

	tcase_add_test(tc_find_chapter_by_numbering, test_find_chapter_by_numbering);
	tcase_add_test(tc_parse_chapters, test_parse_chapters);
	tcase_add_test(tc_print_chapter_no_color, test_print_chapter_no_color);

	suite_add_tcase(s, tc_find_chapter_by_numbering);
	suite_add_tcase(s, tc_parse_chapters);
	suite_add_tcase(s, tc_print_chapter_no_color);

	return s;
}
