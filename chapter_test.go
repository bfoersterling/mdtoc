package main

import (
	"os"
	"testing"
)

func Test_print_chapter(t *testing.T) {
	print_chapter("test_files/test.md", "2.1.", os.Stdout)
}
