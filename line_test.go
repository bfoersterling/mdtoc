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

	// 3 - lines starting with 6 num signs should be recognized as heading
	input = "###### this is a header\n"

	if !is_heading(input) {
		t.Fatalf("input %q should be recognized as a heading.\n", input)
	}
}

func Test_search_section(t *testing.T) {
	// 1
	lines := get_lines("test_files/test.md", "off")

	section_lines, err := search_section(lines, "1.1.")

	if err != nil {
		t.Fatalf("search_section failed with err:\n"+
			"%v\n", err)
	}

	if section_lines[0].raw() != "## header2" {
		t.Fatalf("section_lines[0].raw() should be '## header2'!\n")
	}

	if section_lines[len(section_lines)-1].raw() != "" {
		t.Fatalf("Last raw line of section_lines should be ''.\n")
	}

	// 2 - pretty numbering doesnt exist
	lines = get_lines("test_files/test.md", "off")

	section_lines, err = search_section(lines, "2.")

	if err == nil {
		t.Fatalf("search_section should fail with error 'Section not found'!")
	}

	// 3 - last section
	lines = get_lines("test_files/test.md", "off")

	section_lines, err = search_section(lines, "1.2.")

	if section_lines[0].raw() != "## a second header2" {
		t.Fatalf("section_lines[0].raw() should  be '## a second header2'.\n")
	}

	if section_lines[len(section_lines)-1].raw() != "```" {
		t.Fatalf("Last line of section_lines should be '```'.\n")
	}
}
