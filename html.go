package main

import (
	"bytes"
	"fmt"
	"io"
	"strconv"
)

func print_html(files []string, writer io.Writer) (err error) {
	md_content, err := aggregate_md(files)

	if err != nil {
		return
	}

	reader := bytes.NewReader(md_content)

	html := generate_html(reader)

	fmt.Fprintf(writer, html)

	return
}

func generate_html(reader io.Reader) (html string) {
	lines := parse_lines(reader, "off")

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
