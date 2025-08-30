package main

import (
	"fmt"
	"io"
	"path"
)

func print_chapter(file_path string, chapter string, writer io.Writer) (err error) {
	fmt.Fprintf(writer, "%s\n", path.Base(file_path))

	headings, lines := get_headings_and_lines(file_path)

	start_header := get_heading_by_pnumber(headings, chapter)

	if start_header == (heading{}) {
		fmt.Fprintf(writer, "Heading number %s does not exist.\n", chapter)
		err = fmt.Errorf("Heading number %s does not exist.\n", chapter)
		return
	}

	fmt.Fprintf(writer, "%s\n", chapter)
	fmt.Printf("%v\n", headings)

	for i, v := range lines {
		fmt.Fprintf(writer, "%d: %s\n", i, v)
	}

	fmt.Fprintf(writer, "%+v\n", start_header)

	return
}
