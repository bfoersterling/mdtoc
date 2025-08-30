package main

import (
	"fmt"
	"io"
	"path"
)

func print_chapter(file_path string, chapter string, writer io.Writer) {
	fmt.Fprintf(writer, "%s\n", path.Base(file_path))
	fmt.Fprintf(writer, "%s\n", chapter)

	headings, lines := get_headings_and_lines(file_path)

	fmt.Printf("%v\n", headings)
	for i, v := range lines {
		fmt.Printf("%d: %s\n", i, v)
	}
}
