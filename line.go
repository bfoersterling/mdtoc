package main

import "strings"

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

func get_section_end(headings []heading, start_number string) (end_line int) {
	inside_section := false

	for _, v := range headings {
		if v.pretty_numbering == start_number {
			inside_section = true
		}
		if inside_section && !strings.HasPrefix(v.pretty_numbering, start_number) {
			end_line = v.line - 1
			return
		}
	}

	return
}

// search all lines
// and return the first heading that has that pretty_numbering
func search_section_start(lines []line, pretty_numbering string) (start_header heading) {
	for _, v := range lines {
		heading, ok := v.(heading)

		if !ok {
			continue
		}

		if heading.pretty_numbering == pretty_numbering {
			start_header = heading
			return
		}
	}

	return
}

func search_section_end(lines []line, start_number string) (end_line int) {
	inside_section := false

	for _, v := range lines {
		heading, ok := v.(heading)

		if !ok {
			continue
		}

		if heading.pretty_numbering == start_number {
			inside_section = true
		}
		if inside_section && !strings.HasPrefix(heading.pretty_numbering, start_number) {
			end_line = heading.line - 1
			return
		}
	}

	return
}
