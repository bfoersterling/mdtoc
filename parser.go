package main

import (
	"bufio"
	"io"
	"log"
	"os"
	"strings"
)

// get raw heading numbering
// i.e. [0 1 0 2 0 0]
// this output could then be pretty printed by omitting zeros
func get_heading_numberings(current_level int, current_level_count [6]int) (level_count [6]int) {
	level_count = current_level_count

	level_count[current_level-1]++

	return
}

func parse_headings(reader io.Reader) []heading {
	md_scanner := bufio.NewScanner(reader)
	var headings []heading
	is_codeblock := false
	index := 0
	//	var hnumbering [6]int
	// hnum_cursor may have values 0-6
	//	var hnum_cursor int

	md_scanner.Split(bufio.ScanLines)

	for md_scanner.Scan() {
		index++

		md_line := md_scanner.Text()

		if strings.HasPrefix(md_line, "```") {
			is_codeblock = !is_codeblock
		}

		if is_codeblock {
			continue
		}

		if !is_heading(md_line) {
			continue
		}

		hlevel := get_heading_level(md_line)

		/*
			if len(headings) > 0 && hlevel < headings[index-1].level {
				hnum_cursor++
			} else if
			}
		*/

		headings = append(
			headings, heading{md_line, hlevel, index})
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
