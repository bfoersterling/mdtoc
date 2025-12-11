package main

import (
	"fmt"
	"io"
	"os"
	"strings"
	"unicode"

	"github.com/chzyer/readline"
)

func do_interactive(args cli_args) {
	var err error
	var last_chapter string

	var completer = readline.NewPrefixCompleter(
		readline.PcItem("edit"),
		readline.PcItem("help"),
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

	// maybe implement a caching mechanism or prefetch data

	for {
		line, err := rli.Readline()

		if err == io.EOF {
			break
		}

		trimmed_line := strings.TrimSpace(line)

		if trimmed_line == "" {
			continue
		}

		switch {
		case unicode.IsDigit(rune(trimmed_line[0])):
			print_chapter(args.files[0], trimmed_line, args.color, os.Stdout)
			last_chapter = trimmed_line
		case (trimmed_line == "e") || (trimmed_line == "edit"):
			edit_chapter(args.files[0], last_chapter)
		case (trimmed_line == "s") || (trimmed_line == "summary"):
			print_tree(args.files[0], os.Stdout)
		case (trimmed_line == "h") || (trimmed_line == "help"):
			help_imode()
		}
	}
}

func help_imode() {
	fmt.Printf("List of available commands:\n")
	fmt.Printf("[0-9]...(heading numbering)\tprint chapter (i.e. type \"2.1.3\")\n")
	fmt.Printf("e[edit]\t\t\t\tedit chapter that was printed most recently\n")
	fmt.Printf("h[help]\t\t\t\tprint this help\n")
	fmt.Printf("s[ummary]\t\t\tprint toc of markdown file\n")
}
