package main

// TODO: read a file line by line

var version string

func main() {
	flags := get_cli_args()

	flags.evaluate()
}
