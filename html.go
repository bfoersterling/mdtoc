package main

import (
	"fmt"
	"io"
	"os"
	"slices"
	"strconv"
)

func print_html(files []string, writer io.Writer) {
	var lines []line

	if len(files) == 0 {
		lines = parse_lines(os.Stdin, "off")

		html := generate_html(lines)

		fmt.Fprintf(writer, html)

		return
	}

	for _, file := range files {
		new_lines := fetch_lines(file, "off")
		lines = slices.Concat(lines, new_lines)
	}

	html := generate_html(lines)

	fmt.Fprintf(writer, html)
}

func generate_html(lines []line) (html string) {
	for _, line := range lines {
		switch v := line.(type) {
		case heading:
			html += "<h" + strconv.Itoa(v.level) + ">" + v.text + "</h" +
				strconv.Itoa(v.level) + ">"
		case codeline:
			html += "<code>" + v.text + "</code>"
		case nonheading:
			html += "<p>" + v.text + "</p>"
		}

		html += "\n"
	}

	return
}
