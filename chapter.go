package main

import (
	"fmt"
	"io"
	"path"
	"strings"

	fc "github.com/fatih/color"
)

func print_chapter(file_path string, chapter string, color string, writer io.Writer) (err error) {
	if color == "off" {
		fc.NoColor = true
	} else if color == "on" {
		fc.NoColor = false
	}

	// user may enter a heading numbering without a trailing period
	if !strings.HasSuffix(chapter, ".") {
		chapter += "."
	}

	fmt.Fprintf(writer, "%s\n", path.Base(file_path))

	lines := fetch_lines(file_path, color)

	section_lines, err := search_section(lines, chapter)

	if err != nil {
		return
	}

	for _, v := range section_lines {
		fmt.Fprintf(writer, "%s\n", v.pretty())
	}

	return
}
