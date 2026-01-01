package main

import (
	"bufio"
	"bytes"
	"os"
)

// Read all files and convert them to byte slices
// or read from stdin if no files is an empty slice.
func aggregate_md(files []string) (md_content []byte, err error) {
	if len(files) == 0 {
		var buffer bytes.Buffer
		reader := os.Stdin
		stdin_scanner := bufio.NewScanner(reader)

		stdin_scanner.Split(bufio.ScanLines)

		for stdin_scanner.Scan() {
			buffer.WriteString(stdin_scanner.Text())
			buffer.WriteString("\n")
		}

		md_content = buffer.Bytes()

		return
	} else {
		md_content, err = read_files(files)
	}

	return
}

func read_files(files []string) (file_contents []byte, err error) {
	var content []byte

	for _, file := range files {
		content, err = os.ReadFile(file)

		if err != nil {
			return
		}

		file_contents = append(file_contents, content...)
	}
	return
}
