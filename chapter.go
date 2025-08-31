package main

import (
	"fmt"
	"io"
	"path"
	"strings"
)

func print_chapter(file_path string, chapter string, writer io.Writer) (err error) {
	// user may enter a heading numbering without a trailing period
	if !strings.HasSuffix(chapter, ".") {
		chapter += "."
	}

	fmt.Fprintf(writer, "%s\n", path.Base(file_path))

	headings, lines := get_headings_and_lines(file_path)

	start_header := get_heading_by_pnumber(headings, chapter)

	if start_header == (heading{}) {
		fmt.Fprintf(writer, "Heading number %s does not exist.\n", chapter)
		err = fmt.Errorf("Heading number %s does not exist.\n", chapter)
		return
	}

	end_line := get_section_end(headings, start_header.pretty_numbering)

	// if it is the last section print everything until the end
	if end_line == 0 {
		end_line = len(lines)
	}

	for _, v := range lines[start_header.line-1 : end_line] {
		fmt.Fprintf(writer, "%s\n", v)
	}

	return
}
