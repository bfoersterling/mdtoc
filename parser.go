package main

import (
	"bufio"
	"io"
	"log"
	"os"
	"strings"
)

func parse_headings(reader io.Reader) []heading {
	md_scanner := bufio.NewScanner(reader)
	var headings []heading
	is_codeblock := false
	index := 1

	md_scanner.Split(bufio.ScanLines)

	for md_scanner.Scan() {
		md_line := md_scanner.Text()

		if strings.HasPrefix(md_line, "```") {
			is_codeblock = !is_codeblock
		}

		if !is_codeblock && is_heading(md_line) {
			headings = append(
				headings, heading{md_line, get_heading_level(md_line), (index)})
		}
		index++
	}

	return headings
}

func get_headings(file_name string) []heading {
	file_handle, err := os.Open(file_name)

	if err != nil {
		log.Fatal(err)
	}

	headings := parse_headings(file_handle)

	return headings
}
