package main

import "strings"

type heading struct {
	text  string
	level int
	line  int
}

func is_heading(line string) bool {
	if strings.HasPrefix(line, "#") {
		return true
	} else {
		return false
	}
}

func get_heading_level(heading_text string) int {
	level := 0
	for i := 0; heading_text[i] == '#'; i++ {
		level += 1
		if i == (len(heading_text) - 1) {
			break
		}
	}
	return level
}
