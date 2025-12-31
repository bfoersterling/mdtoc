package main

import (
	"fmt"
	"io"
)

func print_html(file_path string, writer io.Writer) {
	lines := fetch_lines(file_path, "off")

	html := generate_html(lines)

	fmt.Fprintf(writer, html)
}

func generate_html(lines []line) (html string) {
	for _, line := range lines {
		switch v := line.(type) {
		case heading:
			html += "<h" + string(v.level) + ">" + v.text + "</h" +
				string(v.level)
		case codeline:
			html += "<code>" + v.text + "</code>"
		case nonheading:
			html += "<p>" + v.text + "</p>"
		}

		html += "\n"
	}

	return
}
