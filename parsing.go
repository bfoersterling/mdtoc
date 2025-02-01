package main

import (
	"log"
	"os"
	"strings"
)

func parse_file(file_name string) []heading {
	var headings []heading

	file_content_raw, err := os.ReadFile(file_name)

	if err != nil {
		log.Fatal(err)
	}

	file_lines := strings.Split(string(file_content_raw), "\n")

	is_codeblock := false

	for index, value := range file_lines {
		if strings.HasPrefix(value, "```") {
			is_codeblock = !is_codeblock
		}

		if !is_codeblock && is_heading(value) {
			headings = append(
				headings, heading{value, get_heading_level(value), (index + 1)})
		}
	}
	return headings
}
