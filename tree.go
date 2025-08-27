package main

import (
	"fmt"
	"path"
)

func get_child_indices(header_index int, headings []heading) []int {
	var children []int

	if header_index == -1 {
		children = get_root_children(headings)
		return children
	}

	parent_level := headings[header_index].level

	if header_index >= (len(headings) - 1) {
		return nil
	}

	next_level := headings[header_index+1].level

	child_level := 0

	if next_level <= parent_level {
		return nil
	} else {
		child_level = next_level
	}
	cur_level := 0

	for i := (header_index + 1); i < len(headings); i++ {
		cur_level = headings[i].level

		if cur_level == child_level {
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

func print_tree(file_name string) {
	var headings []heading = nil

	headings = get_headings(file_name)

	fmt.Println(path.Base(file_name))

	tree(-1, "", headings)
}

func tree(index int, prefix string, headings []heading) {
	children := get_child_indices(index, headings)

	for index, child_index := range children {
		pretty_numbering := pretty_print_numbering(headings[child_index].levels)

		if index == (len(children) - 1) {
			fmt.Printf("%s %s %s (%d)\n", prefix+"`--", pretty_numbering, headings[child_index].text, headings[child_index].line)
			tree(child_index, prefix+"    ", headings)
		} else {
			fmt.Printf("%s %s %s (%d)\n", prefix+"|--", pretty_numbering, headings[child_index].text, headings[child_index].line)
			tree(child_index, prefix+"|   ", headings)
		}
	}
}
