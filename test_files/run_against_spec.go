package main

import (
	"encoding/json"
	"fmt"
	"os"
)

type example struct {
	Id        int    `json:"example"`
	Markdown  string `json:"markdown"`
	Html      string `json:"html"`
	StartLine int    `json:"start_line"`
	EndLine   int    `json:"end_line"`
	Section   string `json:"section"`
}

func main() {
	fmt.Printf("Running mdtoc against cmark spec.\n")

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
		fmt.Printf("%+v\n", example)
	}
}
