package main

import (
	"fmt"
	"io"
	"path"
	"slices"
)

func get_child_indices(header_index int, headings []heading) []int {
	var children []int

	if header_index == -1 {
		children = get_root_children(headings)
		return children
	}

	if header_index >= (len(headings) - 1) {
		return nil
	}

	parent := headings[header_index]

	next_level := headings[header_index+1].level

	child_level := 0

	if next_level <= parent.level {
		return nil
	} else {
		child_level = next_level
	}
	cur_level := 0

	for i := (header_index + 1); i < len(headings); i++ {
		cur_level = headings[i].level

		if is_direct_child(parent, headings[i]) {
			children = append(children, i)
		}

		if cur_level < child_level {
			break
		}
	}

	return children
}

func get_root_children(headings []heading) []int {
	var root_children []int

	lowest_level := 0
	for index, value := range headings {
		cur_level := value.level
		if index == 0 || cur_level <= lowest_level {
			root_children = append(root_children, index)
			lowest_level = cur_level
		}
	}

	return root_children
}

// Out of tree children are also considered direct children.
// This is intentional.
// Given the subsequent headings
// # root,
// #### foo and
// ## bar,
// foo and bar are both direct children of root.
func is_direct_child(parent heading, child heading) bool {
	// the first numbers until the parent level have to match
	if slices.Compare(child.levels[:parent.level], parent.levels[:parent.level]) != 0 {
		return false
	}

	counter := 0
	for _, v := range child.levels[parent.level:] {
		if v > 0 {
			counter++
		}
	}

	// children of children have more than 1 non-zero element
	// if parent and child are the same counter would be 0
	if counter != 1 {
		return false
	}

	return true
}

func print_tree(file_name string, writer io.Writer) {
	var headings []heading = nil

	// currently we don't need colors when printing the heading tree
	lines := fetch_lines(file_name, "off")

	headings = extract_headings(lines)

	fmt.Fprintln(writer, path.Base(file_name))

	tree(-1, "", headings, writer)
}

func tree(index int, prefix string, headings []heading, writer io.Writer) {
	children := get_child_indices(index, headings)

	for index, child_index := range children {
		if index == (len(children) - 1) {
			fmt.Fprintf(writer,
				"%s %s %s (%d)\n", prefix+"`--", headings[child_index].pretty_numbering, headings[child_index].text, headings[child_index].line)
			tree(child_index, prefix+"    ", headings, writer)
		} else {
			fmt.Fprintf(writer,
				"%s %s %s (%d)\n", prefix+"|--", headings[child_index].pretty_numbering, headings[child_index].text, headings[child_index].line)
			tree(child_index, prefix+"|   ", headings, writer)
		}
	}
}
