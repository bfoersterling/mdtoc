package main

import (
	"bufio"
	"io"
	"log"
	"os"
	"strconv"
	"strings"
)

func extract_headings(lines []line) (headings []heading) {
	for _, line := range lines {
		h, ok := line.(heading)

		if !ok {
			continue
		}

		headings = append(headings, h)
	}

	return
}

// get raw heading numbering
// i.e. [0 1 0 2 0 0]
// this output could then be pretty printed by omitting zeros
// but you need a logic when you switch levels
// when you add a new subheading or when you go up a level to
// finish the section
// just incrementing the current level is not enough
func get_heading_numbers(current_level int, previous_level int, current_level_count [6]int) (level_count [6]int) {
	level_count = current_level_count

	level_count[current_level-1]++

	// going up a heading level includes clearing all child levels
	if current_level < previous_level {
		for i := current_level; i < 6; i++ {
			level_count[i] = 0
		}
	}

	return
}

func pretty_print_numbering(level_count [6]int) (heading_number string) {
	for _, level := range level_count {
		if level == 0 {
			continue
		}
		heading_number += strconv.Itoa(level) + "."
	}

	return
}

func parse_lines(reader io.Reader, color_config string) []line {
	md_scanner := bufio.NewScanner(reader)
	is_codeblock := false
	index := 0
	var curr_hlevel int
	var prev_hlevel int
	var hnumbers [6]int
	var pretty_numbering string
	var lines []line

	md_scanner.Split(bufio.ScanLines)

	for md_scanner.Scan() {
		index++

		md_line := md_scanner.Text()

		if strings.HasPrefix(md_line, "```") {
			is_codeblock = !is_codeblock
		}

		if is_codeblock || !is_heading(md_line) {
			nh := nonheading{
				line: index,
				text: md_line,
			}

			lines = append(lines, nh)
			continue
		}

		prev_hlevel = curr_hlevel
		curr_hlevel = get_heading_level(md_line)

		hnumbers = get_heading_numbers(curr_hlevel, prev_hlevel, hnumbers)

		pretty_numbering = pretty_print_numbering(hnumbers)

		h := heading{
			level:            curr_hlevel,
			levels:           hnumbers,
			line:             index,
			pretty_numbering: pretty_numbering,
			text:             md_line,
		}

		lines = append(lines, h)
	}

	return lines
}

func fetch_lines(file_name string, color string) (lines []line) {
	file_handle, err := os.Open(file_name)

	if err != nil {
		log.Fatal(err)
	}

	lines = parse_lines(file_handle, color)

	return
}
