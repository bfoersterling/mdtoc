package main

import (
	"log"
	"testing"
)

func Test_extract_atx_heading_text(t *testing.T) {
	// 1
	input := "####\tfoo"
	expected_output := "foo"

	output := extract_atx_heading_text(input)

	if output != expected_output {
		t.Fatalf("output and expected_output differ.\n"+
			"output:\n%q\nexpected_output:\n%q", output, expected_output)
	}

	// 2
	input = "# foo"
	expected_output = "foo"

	output = extract_atx_heading_text(input)

	if output != expected_output {
		t.Fatalf("output and expected_output differ.\n"+
			"output:\n%q\nexpected_output:\n%q", output, expected_output)
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

func Test_search_section(t *testing.T) {
	// 1
	lines := fetch_lines("test_files/test.md", "off")

	section_lines, err := search_section(lines, "1.1.")

	if err != nil {
		t.Fatalf("search_section failed with err:\n"+
			"%v\n", err)
	}

	if section_lines[0].raw() != "header2" {
		t.Fatalf("section_lines[0].raw() should be 'header2'!\n")
	}

	if section_lines[len(section_lines)-1].raw() != "" {
		t.Fatalf("Last raw line of section_lines should be ''.\n")
	}

	// 2 - pretty numbering doesnt exist
	lines = fetch_lines("test_files/test.md", "off")

	section_lines, err = search_section(lines, "2.")

	if err == nil {
		t.Fatalf("search_section should fail with error 'Section not found'!")
	}

	// 3 - last section
	lines = fetch_lines("test_files/test.md", "off")

	section_lines, err = search_section(lines, "1.2.")

	if section_lines[0].raw() != "a second header2" {
		t.Fatalf("section_lines[0].raw() should  be 'a second header2'.\n")
	}

	if section_lines[len(section_lines)-1].raw() != "```" {
		t.Fatalf("Last line of section_lines should be '```'.\n")
	}

	// 4 - no trailing dot

	lines = fetch_lines("test_files/test.md", "off")

	section_lines, err = search_section(lines, "1.1")

	if err != nil {
		t.Fatalf("search_section failed with err:\n"+
			"%v\n", err)
	}
}

func Benchmark_search_section(b *testing.B) {
	lines := fetch_lines("test_files/audio.md", "on")

	for i := 0; i < b.N; i++ {
		search_section(lines, "5.")
	}
}
