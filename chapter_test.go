package main

import (
	"bytes"
	"testing"
)

func Test_print_chapter(t *testing.T) {
	// 1 - heading numbering does not exist
	test_buffer := bytes.NewBuffer([]byte(""))
	err := print_chapter("test_files/test.md", "2.1.", test_buffer)

	if err == nil {
		t.Fatalf("The heading 2.1. should not exist in test_files/test.md.\n")
	}

	// 2 - heading does exist
	test_buffer.Reset()
	err = print_chapter("test_files/test.md", "1.1.", test_buffer)

	if err != nil {
		t.Fatalf("print_chapter should not return an error here.\n")
	}

	expected_result := "test.md\n" +
		"1.1. ## header2\n" +
		"\n" +
		"1.1.1. #### header4\n" +
		"\n" +
		"some text.\n" +
		"\n" +
		"```bash\n" +
		"echo 'hi'\n" +
		"```\n" +
		"\n" +
		"1.1.2. #### another header4\n" +
		"\n" +
		"more text.\n" +
		"\n" +
		"1.1.3. ####third header4\n" +
		"\n" +
		"1.1.4. ####fourth #header4\n" +
		"\n" +
		" ## this is not a header anymore\n" +
		"\n" +
		"1.1.4.1. ##### header5\n" +
		"\n"

	if test_buffer.String() != expected_result {
		t.Fatalf("test_buffer and expected_result differ.\n"+
			"test_buffer:\n%s\nexpected_result:\n%s\n", test_buffer, expected_result)
	}
}
