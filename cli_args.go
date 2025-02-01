package main

import (
	"flag"
	"fmt"
	"os"
)

type cli_args struct {
	file_path string
	version   bool
}

func usage() {
	fmt.Printf("Usage: mdtoc [FILE]\n" +
		"\n-V, --version\tprint version\n")
	os.Exit(1)
}

func get_cli_args() cli_args {
	args := cli_args{}

	flag.BoolVar(&args.version, "V", false, "print version")
	flag.BoolVar(&args.version, "version", false, "print version")

	flag.Usage = usage

	flag.Parse()

	if len(os.Args) == 1 {
		usage()
	}

	non_flag_args := flag.Args()

	if len(non_flag_args) == 1 {
		args.file_path = non_flag_args[0]
	} else {
		args.file_path = ""
	}

	return args
}

func (args cli_args) evaluate() {
	if args.version {
		fmt.Printf("%s %s, commit: %s, build at: %s.\n", os.Args[0], version, commit, date)
		os.Exit(0)
	}

	if args.file_path == "" {
		fmt.Printf("Empty file path.\n\n")
		flag.Usage()
	}

	print_tree(args.file_path)
}
