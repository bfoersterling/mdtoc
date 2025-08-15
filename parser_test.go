package main

import (
	"strings"
	"testing"
)

func Test_get_heading_numbering(t *testing.T) {
	// 1
	current_level := 1
	current_lc := [6]int{0, 0, 0, 0, 0, 0}

	expected_result := [6]int{1, 0, 0, 0, 0, 0}

	test_lc := get_heading_numberings(current_level, current_lc)

	if test_lc != expected_result {
		t.Fatalf("test_lc and expected_result differ!\n"+
			"test_lc:\n%v\nexpected_result:\n%v\n", test_lc, expected_result)
	}

	// 2
	current_level = 6
	current_lc = [6]int{1, 2, 0, 8, 0, 1}
	expected_result = [6]int{1, 2, 0, 8, 0, 2}

	test_lc = get_heading_numberings(current_level, current_lc)

	if test_lc != expected_result {
		t.Fatalf("test_lc and expected_result differ!\n"+
			"test_lc:\n%v\nexpected_result:\n%v\n", test_lc, expected_result)
	}
}

func Test_parse_headings(t *testing.T) {
	// 1
	reader := strings.NewReader("## first section\n\n" +
		"#### about\n\n" +
		"#### details\n\n" +
		"## second section\n\n" +
		"#### tutorial\n\n" +
		"#### insights\n")

	test_headings := parse_headings(reader)

	expected_headings := []heading{
		heading{
			text:  "## first section",
			level: 2,
			line:  1,
		},
		heading{
			text:  "#### about",
			level: 4,
			line:  3,
		},
		heading{
			text:  "#### details",
			level: 4,
			line:  5,
		},
		heading{
			text:  "## second section",
			level: 2,
			line:  7,
		},
		heading{
			text:  "#### tutorial",
			level: 4,
			line:  9,
		},
		heading{
			text:  "#### insights",
			level: 4,
			line:  11,
		},
	}

	for i, _ := range test_headings {
		if test_headings[i] != expected_headings[i] {
			t.Fatalf("test_heading[%d] and expected_heading[%d] differ!\n"+
				"test_heading:\n%+v\nexpected_heading:\n%+v\n", i, i, test_headings[i], expected_headings[i])
		}
	}
}
