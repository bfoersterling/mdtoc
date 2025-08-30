package main

import (
	"log"
	"testing"
)

func Test_get_heading_by_pnumber(t *testing.T) {
	// 1
	headings := get_headings("test_files/test.md")

	expected_result := heading{
		text:             "## header2",
		level:            2,
		line:             3,
		levels:           [6]int{1, 1, 0, 0, 0, 0},
		pretty_numbering: "1.1.",
	}

	test_result := get_heading_by_pnumber(headings, "1.1.")

	if test_result != expected_result {
		t.Fatalf("test_result and expected_result differ.\n"+
			"%+v\n%+v\n", test_result, expected_result)
	}
}

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

	// 3 - lines starting with 6 num signs should be recognized as heading
	input = "###### this is a header\n"

	if !is_heading(input) {
		t.Fatalf("input %q should be recognized as a heading.\n", input)
	}
}
