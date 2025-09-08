package main

import (
	"strings"
	"testing"
)

func Test_pretty_print_heading_numbering(t *testing.T) {
	// 1
	hnum := [6]int{1, 2, 0, 8, 0, 2}

	pp_hnum := pretty_print_numbering(hnum)

	if pp_hnum != "1.2.8.2." {
		t.Fatalf("pp_hnum should be 1.2.8.2.\n")
	}
}

func Test_get_heading_numbers(t *testing.T) {
	// 1
	current_level := 1
	current_lc := [6]int{0, 0, 0, 0, 0, 0}

	expected_result := [6]int{1, 0, 0, 0, 0, 0}

	test_lc := get_heading_numbers(current_level, current_lc)

	if test_lc != expected_result {
		t.Fatalf("test_lc and expected_result differ!\n"+
			"test_lc:\n%v\nexpected_result:\n%v\n", test_lc, expected_result)
	}

	// 2
	current_level = 6
	current_lc = [6]int{1, 2, 0, 8, 0, 1}
	expected_result = [6]int{1, 2, 0, 8, 0, 2}

	test_lc = get_heading_numbers(current_level, current_lc)

	if test_lc != expected_result {
		t.Fatalf("test_lc and expected_result differ!\n"+
			"test_lc:\n%v\nexpected_result:\n%v\n", test_lc, expected_result)
	}

	// 3 - move up heading levels
	current_level = 2
	current_lc = [6]int{0, 1, 0, 3, 0, 0}
	expected_result = [6]int{0, 2, 0, 0, 0, 0}

	test_lc = get_heading_numbers(current_level, current_lc)

	if test_lc != expected_result {
		t.Fatalf("test_lc and expected_result differ!\n"+
			"test_lc:\n%v\nexpected_result:\n%v\n", test_lc, expected_result)
	}

	// 4 - move up heading levels with more filled levels in between
	current_level = 2
	current_lc = [6]int{1, 2, 0, 5, 0, 2}
	expected_result = [6]int{1, 3, 0, 0, 0, 0}

	test_lc = get_heading_numbers(current_level, current_lc)

	if test_lc != expected_result {
		t.Fatalf("test_lc and expected_result differ!\n"+
			"test_lc:\n%v\nexpected_result:\n%v\n", test_lc, expected_result)
	}
}

func Benchmark_fetch_lines(b *testing.B) {
	for i := 0; i < b.N; i++ {
		fetch_lines("test_files/audio.md", "off")
	}
}

func Test_parse_lines(t *testing.T) {
	// 1
	reader := strings.NewReader("## first section\n\n" +
		"#### about\n\n" +
		"#### details\n\n" +
		"## second section\n\n" +
		"#### tutorial\n\n" +
		"#### insights\n")

	lines := parse_lines(reader, "off")

	expected_headings := []heading{
		heading{
			text:             "## first section",
			level:            2,
			line:             1,
			levels:           [6]int{0, 1, 0, 0, 0, 0},
			pretty_numbering: "1.",
		},
		heading{
			text:             "#### about",
			level:            4,
			line:             3,
			levels:           [6]int{0, 1, 0, 1, 0, 0},
			pretty_numbering: "1.1.",
		},
		heading{
			text:             "#### details",
			level:            4,
			line:             5,
			levels:           [6]int{0, 1, 0, 2, 0, 0},
			pretty_numbering: "1.2.",
		},
		heading{
			text:             "## second section",
			level:            2,
			line:             7,
			levels:           [6]int{0, 2, 0, 0, 0, 0},
			pretty_numbering: "2.",
		},
		heading{
			text:             "#### tutorial",
			level:            4,
			line:             9,
			levels:           [6]int{0, 2, 0, 1, 0, 0},
			pretty_numbering: "2.1.",
		},
		heading{
			text:             "#### insights",
			level:            4,
			line:             11,
			levels:           [6]int{0, 2, 0, 2, 0, 0},
			pretty_numbering: "2.2.",
		},
	}

	test_headings := extract_headings(lines)

	for i, _ := range test_headings {
		if test_headings[i] != expected_headings[i] {
			t.Fatalf("test_heading[%d] and expected_heading[%d] differ!\n"+
				"test_heading:\n%+v\nexpected_heading:\n%+v\n", i, i, test_headings[i], expected_headings[i])
		}
	}
}
