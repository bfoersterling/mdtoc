package main

import (
	"flag"
	"fmt"
	"os"
)

type cli_args struct {
	chapter     string
	color       string
	edit        string
	files       []string
	html        bool
	interactive bool
	version     bool
}

func usage() {
	fmt.Printf("Usage: mdtoc [OPTIONS] [FILE]\n" +
		"\n-C, --color [off/auto/on]\tenable/disable colors\n" +
		"-c, --chapter [CHAPTER]\t\tprint chapter under header\n" +
		"-e, --edit [CHAPTER]\t\tedit chapter under header\n" +
		"-i, --interactive\t\tinteractive mode\n" +
		"--html\t\thtml output (for spec tests)\n" +
		"-V, --version\t\t\tprint version\n")
	os.Exit(1)
}

func get_cli_args() cli_args {
	args := cli_args{}

	flag.StringVar(&args.chapter, "c", "", "print chapter")
	flag.StringVar(&args.chapter, "chapter", "", "print chapter")
	flag.StringVar(&args.color, "C", "auto", "set color to on, off, or auto")
	flag.StringVar(&args.color, "color", "auto", "set color to on, off, or auto")
	flag.StringVar(&args.edit, "e", "", "edit the chapter")
	flag.StringVar(&args.edit, "edit", "", "edit the chapter")
	flag.BoolVar(&args.interactive, "i", false, "interactive mode")
	flag.BoolVar(&args.interactive, "interactive", false, "interactive mode")
	flag.BoolVar(&args.html, "html", false, "html output (for spec tests)")
	flag.BoolVar(&args.version, "V", false, "print version")
	flag.BoolVar(&args.version, "version", false, "print version")

	flag.Usage = usage

	flag.Parse()

	args.files = flag.Args()

	return args
}

func (args cli_args) evaluate() {
	if args.version {
		fmt.Printf("%s %s, commit: %s, build at: %s.\n", os.Args[0], version, commit, date)
		os.Exit(0)
	}

	if len(args.files) == 0 && !args.html {
		fmt.Printf("No input files were provided.\n\n")
		flag.Usage()
	}

	const (
		summary_mode = iota
		interactive_mode
		chapter_mode
		edit_mode
		html_mode
	)

	var mode int = summary_mode

	switch {
	case args.chapter != "":
		mode = chapter_mode
	case args.edit != "":
		mode = edit_mode
	case args.interactive:
		mode = interactive_mode
	case args.html:
		mode = html_mode
	}

	if mode == chapter_mode {
		for _, file_path := range args.files {
			err := print_chapter(file_path, args.chapter, args.color, os.Stdout)

			if err != nil {
				fmt.Printf("%v\n", err)
				os.Exit(1)
			}
		}
		os.Exit(0)
	}

	if mode == edit_mode {
		err := edit_chapter(args.files[0], args.edit)

		if err != nil {
			fmt.Printf("%v\n", err)
			os.Exit(1)
		}

		os.Exit(0)
	}

	if mode == interactive_mode {
		do_interactive(args)
		os.Exit(0)
	}

	if mode == html_mode {
		print_html(args.files, os.Stdout)
		os.Exit(0)
	}

	for _, file_path := range args.files {
		print_tree(file_path, os.Stdout)
	}
}
