package main

import (
	"fmt"
	"io"
	"path"
	"strings"
)

func print_chapter(file_path string, args cli_args, writer io.Writer) (err error) {
	chapter := args.chapter

	// user may enter a heading numbering without a trailing period
	if !strings.HasSuffix(chapter, ".") {
		chapter += "."
	}

	fmt.Fprintf(writer, "%s\n", path.Base(file_path))

	lines := get_lines(file_path, args.color)

	start_line, err := search_section_start(lines, chapter)

	if err != nil {
		fmt.Fprintf(writer, "%s\n", err.Error())
		fmt.Fprintf(writer, "Heading number %s does not exist.\n", chapter)
		err = fmt.Errorf("Heading number %s does not exist.\n", chapter)
		return
	}

	end_line, err := search_section_end(lines, chapter)

	if err != nil {
		fmt.Fprintf(writer, "%s\n", err.Error())
		return
	}

	// if it is the last section print everything until the end
	if end_line == 0 {
		end_line = len(lines)
	}

	for _, v := range lines[start_line-1 : end_line] {
		fmt.Fprintf(writer, "%s\n", v.pretty(args.color))
	}

	return
}
