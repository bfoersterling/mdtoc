package main

// TODO: read a file line by line

import (
	"fmt"
	"log"
	"os"
	"path"
	"strings"
)

func parse_file(file_name string) []heading {
	var headings []heading

	file_content_raw, err := os.ReadFile(file_name)

	if err != nil {
		log.Fatal(err)
	}

	file_lines := strings.Split(string(file_content_raw), "\n")

	is_codeblock := false

	for index, value := range file_lines {
		if strings.HasPrefix(value, "```") {
			is_codeblock = !is_codeblock
		}

		if !is_codeblock && is_heading(value) {
			headings = append(
				headings, heading{value, Get_heading_level(value), (index + 1)})
		}
	}
	return headings
}

func usage() {
	fmt.Printf("Usage: mdtoc [FILE]\n")
	os.Exit(1)
}

func main() {
	var file_name string = ""

	if len(os.Args) == 2 {
		file_name = os.Args[1]
	} else {
		usage()
	}

	var headings []heading = nil

	headings = parse_file(file_name)

	fmt.Println(path.Base(file_name))
	tree(-1, "", headings)
}
