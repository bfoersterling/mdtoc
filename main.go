package main

// TODO: read a file line by line

var (
	version = "dev"
	commit  = "none"
	date    = "unknown"
)

func main() {
	flags := get_cli_args()

	flags.evaluate()
}
