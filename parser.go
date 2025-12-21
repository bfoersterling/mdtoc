package main

import (
	"bufio"
	"io"
	"log"
	"os"
	"strconv"
	"strings"

	tree_sitter_md "github.com/tree-sitter-grammars/tree-sitter-markdown/bindings/go"
	tree_sitter "github.com/tree-sitter/go-tree-sitter"
)

const (
	STATE_NORMAL = iota
	STATE_CODEBLOCK
)

func extract_headings(lines []line) (headings []heading) {
	for _, line := range lines {
		h, ok := line.(heading)

		if !ok {
			continue
		}

		headings = append(headings, h)
	}

	return
}

func fetch_lines(file_name string, color string) (lines []line) {
	file_handle, err := os.Open(file_name)

	if err != nil {
		log.Fatal(err)
	}

	lines = parse_lines(file_handle, color)

	return
}

// tree-sitter engine
// Caller has to Close() the returned tree.
func fetch_syntax_tree(file_name string) (tree *tree_sitter.Tree, err error) {
	var source []byte

	file, err := os.Open(file_name)

	if err != nil {
		return
	}

	defer file.Close()

	_, err = file.Read(source)

	if err != nil {
		return
	}

	parser := tree_sitter.NewParser()

	defer parser.Close()

	err = parser.SetLanguage(tree_sitter.NewLanguage(tree_sitter_md.Language()))

	if err != nil {
		return
	}

	tree = parser.Parse(source, nil)

	return
}

// Get raw heading numbering.
// i.e. [0 1 0 2 0 0]
// This output can then be pretty printed by omitting zeros.
func get_heading_numbers(current_level int, current_level_count [6]int) (level_count [6]int) {
	level_count = current_level_count

	level_count[current_level-1]++

	// clear lower levels
	for i := current_level; i < 6; i++ {
		level_count[i] = 0
	}

	return
}

func is_codeblock_toggle(line string) bool {
	return strings.HasPrefix(line, "```")
}

func pretty_print_numbering(level_count [6]int) (heading_number string) {
	for _, level := range level_count {
		if level == 0 {
			continue
		}
		heading_number += strconv.Itoa(level) + "."
	}

	return
}

func parse_lines(reader io.Reader, color_config string) []line {
	md_scanner := bufio.NewScanner(reader)
	index := 0
	var hnumbers [6]int
	var lines []line
	state := STATE_NORMAL

	md_scanner.Split(bufio.ScanLines)

	for md_scanner.Scan() {
		index++

		md_line := md_scanner.Text()

		switch state {
		case STATE_CODEBLOCK:
			state = scan_codeblock(state, md_line, &lines)
		case STATE_NORMAL:
			state = scan_normal(state, md_line, &hnumbers, &lines)
		}
	}

	return lines
}

func scan_codeblock(current_state int, raw_line string, lines *[]line) (new_state int) {
	new_state = current_state

	if strings.HasPrefix(raw_line, "```") {
		new_state = STATE_NORMAL
		nh := nonheading{
			line: len(*lines) + 1,
			text: raw_line,
		}
		*lines = append(*lines, nh)
		return
	}

	c := codeline{
		line: len(*lines) + 1,
		text: raw_line,
	}

	*lines = append(*lines, c)

	return
}

func scan_normal(current_state int, raw_line string, hnumbers *[6]int, lines *[]line) (new_state int) {
	if is_codeblock_toggle(raw_line) {
		new_state = STATE_CODEBLOCK
	}

	if !is_heading(raw_line) {
		nh := nonheading{
			line: len(*lines) + 1,
			text: raw_line,
		}
		*lines = append(*lines, nh)
		return
	}

	heading_level := get_heading_level(raw_line)

	*hnumbers = get_heading_numbers(heading_level, *hnumbers)

	pretty_numbering := pretty_print_numbering(*hnumbers)

	h := heading{
		level:            heading_level,
		levels:           *hnumbers,
		line:             len(*lines) + 1,
		pretty_numbering: pretty_numbering,
		text:             raw_line,
	}
	*lines = append(*lines, h)

	return
}
