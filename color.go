package main

import "os"

func end_color(color_config string) string {
	if !use_color(color_config) {
		return ""
	}
	return "\033[0m"
}

func is_colorterminal() bool {
	colorterm := os.Getenv("COLORTERM")
	no_color := os.Getenv("NO_COLOR")

	if no_color == "1" {
		return false
	}

	if colorterm == "truecolor" || colorterm == "24bit" {
		return true
	}

	return false
}

func start_color_green(color_config string) string {
	if !use_color(color_config) {
		return ""
	}
	return "\033[0;32m"
}

func use_color(color_config string) bool {
	switch color_config {
	case "off":
		return false
	case "on":
		return true
	case "auto":
		return is_colorterminal()
	}
	return false
}
