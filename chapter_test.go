package main

import (
	"bytes"
	"fmt"
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

	fmt.Printf("%v\n", test_buffer)
}
