#include "unit_testing.h"

#include <stdlib.h>

#include "../pretty_printer.c"

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

Suite*
pretty_printing_suite(void)
{
	Suite* s = suite_create("pretty_printing");

	TCase* tc_pretty_print_heading_levels = tcase_create("test_pretty_print_heading_levels");

	tcase_add_test(tc_pretty_print_heading_levels, test_pretty_print_heading_levels);

	suite_add_tcase(s, tc_pretty_print_heading_levels);

	return s;
}
