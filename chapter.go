package main

import (
	"fmt"
	"io"
	"os"
	"os/exec"
	"path"
	"strconv"
	"strings"

	fc "github.com/fatih/color"
)

func edit_chapter(file_path string, chapter string) (err error) {
	editor := os.Getenv("EDITOR")

	if len(editor) == 0 {
		editor = "vim"
	}

	// vim, nvim and helix support the +lineno syntax
	if editor != "vim" && editor != "nvim" && editor != "helix" {
		err = fmt.Errorf("The editor %q is not supported.", editor)
		return err
	}

	lines := fetch_lines(file_path, "off")

	chapter_lines, err := search_section(lines, chapter)

	if err != nil {
		return err
	}

	cmd_args := "+" + strconv.Itoa(chapter_lines[0].number())

	cmd := exec.Command(editor, file_path, cmd_args)

	cmd.Stdin = os.Stdin
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	cmd.Run()

	return err
}

func print_chapter(file_path string, chapter string, color string, writer io.Writer) (err error) {
	if color == "off" {
		fc.NoColor = true
	} else if color == "on" {
		fc.NoColor = false
	}

	// user may enter a heading numbering without a trailing period
	if !strings.HasSuffix(chapter, ".") {
		chapter += "."
	}

	fmt.Fprintf(writer, "%s\n", path.Base(file_path))

	lines := fetch_lines(file_path, color)

	section_lines, err := search_section(lines, chapter)

	if err != nil {
		return
	}

	for _, v := range section_lines {
		fmt.Fprintf(writer, "%s\n", v.pretty())
	}

	return
}
