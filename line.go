package main

import (
	"fmt"
	"strings"
	"unicode"

	fc "github.com/fatih/color"
)

type line interface {
	number() int
	pretty() string
	raw() string
}

type codeline struct {
	line int
	text string
}

func (c codeline) number() int {
	return c.line
}

func (c codeline) pretty() string {
	color := fc.New().Add(fc.BgHiBlack, fc.FgYellow)

	return color.Sprintf("%s", c.text)
}

func (c codeline) raw() string {
	return c.text
}

type atx_heading struct {
	text             string
	level            int
	line             int
	levels           [6]int
	pretty_numbering string
}

func (h atx_heading) number() int {
	return h.line
}

func (h atx_heading) pretty() string {
	return fc.GreenString(h.pretty_numbering + " " + h.text)
}

func (h atx_heading) raw() string {
	return h.text
}

type dashed_line struct {
	line int
	text string
}

func (d dashed_line) number() int {
	return d.line
}

func (d dashed_line) pretty() string {
	color := fc.New().Add(fc.BgBlack, fc.FgBlue)
	return color.Sprintf("%s", d.text)
}

func (d dashed_line) raw() string {
	return d.text
}

type nonheading struct {
	line int
	text string
}

func (nh nonheading) number() int {
	return nh.line
}

func (nh nonheading) pretty() string {
	return nh.text
}

func (nh nonheading) raw() string {
	return nh.text
}

func is_atx_heading(line string) bool {
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

func is_dashed(line string) bool {
	dash_count := 0
	for _, r := range line {
		if r == '-' {
			dash_count++
			continue
		}
		if !unicode.IsSpace(r) {
			return false
		}
	}

	if dash_count > 2 {
		return true
	} else {
		return false
	}
}

func get_atx_heading_level(heading_text string) int {
	level := 0
	for i := 0; heading_text[i] == '#'; i++ {
		level += 1
		if i == (len(heading_text) - 1) {
			break
		}
	}
	return level
}

func extract_atx_heading_text(line string) string {
	line = strings.TrimSpace(line)

	for i, r := range line {
		if r != '#' {
			return strings.TrimSpace(line[i:])
		}
	}

	return line
}

// Search through all lines and identify the start line of the section.
// Then continue to the end of the section and return a slice of the section.
// A trailing dot is not mandatory for "pretty_numbering".
func search_section(lines []line, pretty_numbering string) (section []line, err error) {
	// -1 to distinguish from the first element which would be 0
	start_line := -1
	var end_line int

	if len(lines) == 0 {
		err = fmt.Errorf("Empty line slice was passed.")
		return
	}

	if !strings.HasSuffix(pretty_numbering, ".") {
		pretty_numbering += "."
	}

	// search start line
	for i, v := range lines {
		h, ok := v.(atx_heading)

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
		h, ok := v.(atx_heading)

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
