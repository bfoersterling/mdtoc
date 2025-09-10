package main

import (
	"fmt"
	"io"
	"path"
	"strings"

	fc "github.com/fatih/color"
)

func print_chapter(file_path string, args cli_args, writer io.Writer) (err error) {
	chapter := args.chapter

	if args.color == "off" {
		fc.NoColor = true
	} else if args.color == "on" {
		fc.NoColor = false
	}

	// user may enter a heading numbering without a trailing period
	if !strings.HasSuffix(chapter, ".") {
		chapter += "."
	}

	fmt.Fprintf(writer, "%s\n", path.Base(file_path))

	lines := fetch_lines(file_path, args.color)

	section_lines, err := search_section(lines, chapter)

	if err != nil {
		return
	}

	//for _, v := range lines[start_line-1 : end_line] {
	for _, v := range section_lines {
		fmt.Fprintf(writer, "%s\n", v.pretty())
	}

	return
}
