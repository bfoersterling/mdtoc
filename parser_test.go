package main

import (
	"strings"
	"testing"
)

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
