package main

import (
	"io"
	"os"
	"strings"
	"unicode"

	"github.com/chzyer/readline"
)

func do_interactive(args cli_args) {
	var err error

	var completer = readline.NewPrefixCompleter(
		readline.PcItem("summary"),
	)

	print_tree(args.files[0], os.Stdout)

	rli, err := readline.NewEx(&readline.Config{
		Prompt:          "mdtoc> ",
		InterruptPrompt: "^C",
		EOFPrompt:       "exit",
		AutoComplete:    completer,
	})

	if err != nil {
		panic(err)
	}

	for {
		line, err := rli.Readline()

		if err == io.EOF {
			break
		}

		trimmed_line := strings.TrimSpace(line)

		if trimmed_line == "" {
			continue
		}

		if unicode.IsDigit(rune(trimmed_line[0])) {
			print_chapter(args.files[0], trimmed_line, args.color, os.Stdout)
		}

		if (trimmed_line == "s") || (trimmed_line == "summary") {
			print_tree(args.files[0], os.Stdout)
		}
	}
}
