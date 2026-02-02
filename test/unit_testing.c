#include "unit_testing.h"

int
main(void) {
	int number_failed = 0;

	SRunner* sr = srunner_create(parser_suite());
	srunner_add_suite(sr, chapter_suite());
	srunner_add_suite(sr, pretty_printing_suite());
	srunner_add_suite(sr, stream_suite());
	srunner_add_suite(sr, string_util_suite());

	srunner_run_all(sr, CK_NORMAL);

	number_failed += srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? 0 : 1;
}
