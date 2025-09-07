package main

import (
	"fmt"
	"strings"
)

type line interface {
	number() int
	pretty(string) string
	raw() string
}

type heading struct {
	text             string
	level            int
	line             int
	levels           [6]int
	pretty_numbering string
}

func (h heading) number() int {
	return h.line
}

func (h heading) pretty(color_config string) string {
	return start_color_green(color_config) + h.pretty_numbering + " " + h.text +
		end_color(color_config)
}

func (h heading) raw() string {
	return h.text
}

type nonheading struct {
	line int
	text string
}

func (nh nonheading) number() int {
	return nh.line
}

func (nh nonheading) pretty(color_config string) string {
	return nh.text
}

func (nh nonheading) raw() string {
	return nh.text
}

func is_heading(line string) bool {
	if !strings.HasPrefix(line, "#") {
		return false
	}
	leading_num_signs := 0

	for i, v := range line {
		if v != '#' {
			leading_num_signs = i
			break
		}
	}

	// lines starting with over six pound signs are not headers
	return leading_num_signs <= 6
}

func get_heading_level(heading_text string) int {
	level := 0
	for i := 0; heading_text[i] == '#'; i++ {
		level += 1
		if i == (len(heading_text) - 1) {
			break
		}
	}
	return level
}

// Search through all lines and identify the start line of the section.
// Then continue to the end of the section and return a slice of the section.
func search_section(lines []line, pretty_numbering string) (section []line, err error) {
	// -1 to distinguish from the first element which would be 0
	start_line := -1
	var end_line int

	if len(lines) == 0 {
		err = fmt.Errorf("Empty line slice was passed.")
		return
	}

	// search start line
	for i, v := range lines {
		h, ok := v.(heading)

		if !ok {
			continue
		}

		if h.pretty_numbering == pretty_numbering {
			start_line = i
			break
		}
	}

	if start_line == -1 {
		err = fmt.Errorf("Section %q not found!", pretty_numbering)
		return
	}

	// search end line
	for i, v := range lines[start_line:] {
		h, ok := v.(heading)

		if !ok {
			continue
		}

		if !strings.HasPrefix(h.pretty_numbering, pretty_numbering) {
			end_line = i + start_line
			break
		}
	}

	if end_line == 0 {
		section = lines[start_line:]
		return
	}

	if start_line >= end_line {
		err = fmt.Errorf("start_line can't be bigger than end_line.")
		return
	}

	section = lines[start_line:end_line]

	return
}
