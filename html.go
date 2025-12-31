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

	fmt.Fprintf(writer, string(html))

	return
}

func generate_html(reader io.Reader) (html []byte) {
	var html_buffer bytes.Buffer
	lines := parse_lines(reader, "off")

	for _, line := range lines {
		switch v := line.(type) {
		case heading:
			html_buffer.WriteString("<h")
			html_buffer.WriteString(strconv.Itoa(v.level))
			html_buffer.WriteString(">")
			html_buffer.WriteString(v.text)
			html_buffer.WriteString("</h")
			html_buffer.WriteString(strconv.Itoa(v.level))
			html_buffer.WriteString(">")
		case codeline:
			html_buffer.WriteString("<code>")
			html_buffer.WriteString(v.text)
			html_buffer.WriteString("</code>")
		case nonheading:
			html_buffer.WriteString("<p>")
			html_buffer.WriteString(v.text)
			html_buffer.WriteString("</p>")
		}

		html_buffer.WriteString("\n")
	}

	html = html_buffer.Bytes()

	return
}
