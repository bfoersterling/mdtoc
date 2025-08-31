package main

func start_color_green(color_config string) string {
	return "\033[0;32m"
}

func end_color(color_config string) string {
	return "\033[0m"
}
