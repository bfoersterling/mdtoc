package main

import (
	"fmt"
	"io"
	"os"
	"unicode"
)

func do_interactive(args cli_args) {
	var input string
	var err error

	print_tree(args.files[0], os.Stdout)

	for {
		fmt.Printf("mdtoc> ")

		_, err = fmt.Scanf("%s", &input)

		if err == io.EOF {
			fmt.Printf("\n")
			break
		}

		if input == "" {
			continue
		}

		if unicode.IsDigit(rune(input[0])) {
			print_chapter(args.files[0], input, args.color, os.Stdout)
		}

		input = ""
	}
}
