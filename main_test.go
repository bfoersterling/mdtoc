package main

import (
	"log"
	"testing"
)

func Test_get_heading_level(t *testing.T) {
	level := get_heading_level("# foo")

	if level != 1 {
		log.Fatal("Level should be 1.")
	}

	level = get_heading_level("not a header")

	if level != 0 {
		log.Fatal("A non header should be level 0!")
	}

	level = get_heading_level("#")

	if level != 1 {
		log.Fatal("Level should be 1.")
	}
}
