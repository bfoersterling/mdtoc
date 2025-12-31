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
			"html_10:\n%q\noutput_10:%q\n", html_10, output_10)
	}
}
