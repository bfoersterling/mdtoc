package main

import (
	"bytes"
	"fmt"
	"testing"
)

func Test_generate_html(t *testing.T) {
	// example 10 from spec.txt
	input_10 := []byte("#\tFoo\n")
	html_10 := "<h1>Foo</h1>\n"

	reader_10 := bytes.NewReader(input_10)
	output_10 := generate_html(reader_10)

	fmt.Printf("%s\n", output_10)
	fmt.Printf("Expected %s\n", html_10)
}
