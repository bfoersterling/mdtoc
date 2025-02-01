package main

// TODO: read a file line by line

import (
	"fmt"
	"os"
	"path"
)

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
