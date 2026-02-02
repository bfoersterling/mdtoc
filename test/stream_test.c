#include "unit_testing.h"

#include "../stream.c"

START_TEST(test_count_lines)
{
	char* file_content_1 =
		"## foo\n"
		"Paragraph.\n";

	FILE* file_1 = fmemopen(file_content_1, strlen(file_content_1), "r");

	// Also test that file position remains unchanged.
	fseek(file_1, 2, SEEK_SET);

	int line_count = count_lines(file_1);

	ck_assert_int_eq(line_count, 2);
	ck_assert_int_eq(ftell(file_1), 2);

	fclose(file_1);
}

START_TEST(test_file_size)
{
	// 1
	char* source_1 =
		"## test document\n"
		"Some text.\n";

	FILE* file_1 = fmemopen(source_1, strlen(source_1), "r");

	// Also test that file position remains the same.
	fseek(file_1, 5, SEEK_SET);

	size_t file_size_1 = (size_t)file_size(file_1);

	ck_assert_int_eq(file_size_1, 28);
	ck_assert_int_eq(ftell(file_1), 5);

	fclose(file_1);
}

START_TEST(test_line_end_pos)
{
	// 1
	char* source_1 =
		"## foo\n"
		"#### bar\n";

	FILE* file_1 = fmemopen(source_1, strlen(source_1), "r");

	long pos_1 = line_end_pos(file_1, 1);

	// Warning: ck_assert_int_eq is used for type long here.
	ck_assert_int_eq(pos_1, 7);

	// Line that does not exist.
	pos_1 = line_end_pos(file_1, 10);

	ck_assert_int_eq(pos_1, -1);

	fclose(file_1);
}

START_TEST(test_line_start_pos)
{
	char* source_code =
		"## foo\n"
		"#### subfoo\n";

	FILE* file_1 = fmemopen(source_code, strlen(source_code), "r");

	long pos_1 = line_start_pos(file_1, 2);

	// Warning: ck_assert_int_eq is used for type long here.
	ck_assert_int_eq(pos_1, 7);

	fclose(file_1);
}
END_TEST

Suite*
stream_suite() {
	Suite* s = suite_create("stream");

	TCase* tc_test_count_lines = tcase_create("test_count_lines");
	TCase* tc_test_file_size = tcase_create("test_file_size");
	TCase* tc_test_line_end_pos = tcase_create("test_line_end_pos");
	TCase* tc_test_line_start_pos = tcase_create("test_line_start_pos");

	tcase_add_test(tc_test_count_lines, test_count_lines);
	tcase_add_test(tc_test_file_size, test_file_size);
	tcase_add_test(tc_test_line_end_pos, test_line_end_pos);
	tcase_add_test(tc_test_line_start_pos, test_line_start_pos);

	suite_add_tcase(s, tc_test_count_lines);
	suite_add_tcase(s, tc_test_file_size);
	suite_add_tcase(s, tc_test_line_end_pos);
	suite_add_tcase(s, tc_test_line_start_pos);

	return s;
}
