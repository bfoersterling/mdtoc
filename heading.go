package main

import "strings"

type heading struct {
	text             string
	level            int
	line             int
	levels           [6]int
	pretty_numbering string
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

// get heading by field pretty_numbering
func get_heading_by_pnumber(headings []heading, numbering string) (needle heading) {
	for _, v := range headings {
		if v.pretty_numbering == numbering {
			needle = v
			return
		}
	}

	return
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
