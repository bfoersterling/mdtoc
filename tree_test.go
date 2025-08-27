package main

import (
	"bytes"
	"testing"
)

func Test_print_tree(t *testing.T) {
	// 1
	test_buffer := bytes.NewBuffer([]byte(""))

	expected_result := "test.md\n" +
		"`-- 1. # test markdown doc (1)\n" +
		"    |-- 1.1. ## header2 (3)\n" +
		"    |   |-- 1.1.1. #### header4 (5)\n" +
		"    |   |-- 1.1.2. #### another header4 (13)\n" +
		"    |   |-- 1.1.3. ####third header4 (17)\n" +
		"    |   `-- 1.1.4. ####fourth #header4 (19)\n" +
		"    |       `-- 1.1.4.1. ##### header5 (23)\n" +
		"    `-- 1.2. ## a second header2 (25)\n" +
		"        `-- 1.2.1. #### testing (27)\n"

	print_tree("test_files/test.md", test_buffer)

	if test_buffer.String() != expected_result {
		t.Fatalf("test_buffer.String() and expected result differ!\n"+
			"test_buffer.String():\n%s\nexpected_result:\n%s\n",
			test_buffer.String(), expected_result)
	}

	// 2 - crlf line endings
	test_buffer.Reset()

	expected_result = "crlf.md\n" +
		"`-- 1. # test markdown doc (1)\n" +
		"    |-- 1.1. ## header2 (3)\n" +
		"    |   |-- 1.1.1. #### header4 (5)\n" +
		"    |   |-- 1.1.2. #### another header4 (13)\n" +
		"    |   |-- 1.1.3. ####third header4 (17)\n" +
		"    |   `-- 1.1.4. ####fourth #header4 (19)\n" +
		"    |       `-- 1.1.4.1. ##### header5 (23)\n" +
		"    `-- 1.2. ## a second header2 (25)\n" +
		"        `-- 1.2.1. #### testing (27)\n"

	print_tree("test_files/crlf.md", test_buffer)

	if test_buffer.String() != expected_result {
		t.Fatalf("test_buffer.String() and expected result differ!\n"+
			"test_buffer.String():\n%q\nexpected_result:\n%q\n",
			test_buffer.String(), expected_result)
	}
}
