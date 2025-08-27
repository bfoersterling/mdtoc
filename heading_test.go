package main

import (
	"log"
	"testing"
)

func Test_get_heading_level(t *testing.T) {
	level := get_heading_level("# foo")

	if level != 1 {
		log.Fatal("Level should be 1.")
	}

	level = get_heading_level("not a header")

	if level != 0 {
		log.Fatal("A non header should be level 0!")
	}

	level = get_heading_level("#")

	if level != 1 {
		log.Fatal("Level should be 1.")
	}
}

func Test_is_heading(t *testing.T) {
	// 1
	input := "## foobar\n"

	if !is_heading(input) {
		t.Fatalf("input %q should be categorized as a header.\n", input)
	}

	// 2 - over 6 num signs should not be recognized as headings
	input = "####### level seven headers dont exist\n"

	if is_heading(input) {
		t.Fatalf("input %q should not be categorized as a header.\n", input)
	}
}
