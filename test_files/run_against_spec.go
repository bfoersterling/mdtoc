package main

import (
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"strings"
)

type example struct {
	Id        int    `json:"example"`
	Markdown  string `json:"markdown"`
	Html      string `json:"html"`
	StartLine int    `json:"start_line"`
	EndLine   int    `json:"end_line"`
	Section   string `json:"section"`
}

type result struct {
	ok      int
	failed  int
	errored int
}

func (e *example) run_spec_test(res *result) {
	cmd := exec.Command("../mdtoc", "-html", "-unsafe")

	cmd.Stdin = strings.NewReader(e.Markdown)

	out, err := cmd.CombinedOutput()

	fmt.Printf("DEBUG: %+v\n", cmd)

	if err != nil {
		fmt.Printf("Test %d errored.\n", e.Id)
		fmt.Printf("actual:\n%q\nexpected:\n%q\n", out, e.Html)
		res.errored++
		return
	}

	if string(out) == e.Html {
		fmt.Printf("Test %d successful.\n", e.Id)
		res.ok++
	} else {
		fmt.Printf("Test %d failed.\n", e.Id)
		fmt.Printf("input:\n%q\n", e.Markdown)
		fmt.Printf("actual:\n%q\nexpected:\n%q\n", out, e.Html)
		res.failed++
	}
}

func main() {
	fmt.Printf("Running mdtoc against cmark spec.\n")

	var res result

	spec_bytes, err := os.ReadFile("cmark_spec.json")

	if err != nil {
		panic(err)
	}

	var examples []example

	err = json.Unmarshal(spec_bytes, &examples)

	if err != nil {
		panic(err)
	}

	for _, example := range examples {
		example.run_spec_test(&res)
	}

	fmt.Printf("%+v\n", res)
}
