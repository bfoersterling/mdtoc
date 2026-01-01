package main

import (
	"bytes"
	"testing"
)

func Test_generate_html(t *testing.T) {
	// example 10 from spec.txt
	input_10 := []byte("#\tFoo\n")
	html_10 := []byte("<h1>Foo</h1>\n")

	reader_10 := bytes.NewReader(input_10)
	output_10 := generate_html(reader_10)

	if bytes.Compare(html_10, output_10) != 0 {
		t.Fatalf("html_10 and output_10 differ.\n"+
			"html_10:\n%q\noutput_10:\n%q\n", html_10, output_10)
	}

	// example 62 from spec.txt
	input_62 := []byte("# foo\n## foo\n### foo\n#### foo\n##### foo\n###### foo\n")
	html_62 := []byte("<h1>foo</h1>\n<h2>foo</h2>\n<h3>foo</h3>\n<h4>foo</h4>\n<h5>foo</h5>\n<h6>foo</h6>\n")

	reader_62 := bytes.NewReader(input_62)
	output_62 := generate_html(reader_62)

	if bytes.Compare(html_62, output_62) != 0 {
		t.Fatalf("html_62 and output_62 differ.\n"+
			"html_62:\n%q\noutput_62:\n%q\n", html_62, output_62)
	}
}
