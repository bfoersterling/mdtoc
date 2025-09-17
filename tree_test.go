package main

import (
	"bytes"
	"slices"
	"testing"
)

func Test_get_child_indices(t *testing.T) {
	// 1
	test_lines := fetch_lines("test_files/late_out_of_tree.md", "off")
	test_headings := extract_headings(test_lines)

	test_child_indices := get_child_indices(0, test_headings)

	// out of tree header should also be recognized as a child
	expected_child_indices := []int{1, 2, 3}

	if slices.Compare(test_child_indices, expected_child_indices) != 0 {
		t.Fatalf("test_child_indices and expected_child_indices differ!\n"+
			"test_child_indices:\n%v\n"+
			"expected_child_indices:\n%v\n", test_child_indices, expected_child_indices)
	}
}

func Test_get_root_children(t *testing.T) {
	// 1
	test_lines := fetch_lines("test_files/late_out_of_tree.md", "off")
	test_headings := extract_headings(test_lines)

	root_children_indices := get_root_children(test_headings)

	// there is only one root child, which is a level 1 heading in this case
	if len(root_children_indices) != 1 {
		t.Fatalf("There should be only one root child.")
	}

	if root_children_indices[0] != 0 {
		t.Fatalf("The root child should be the first heading with index 0.\n")
	}
}

func Test_is_direct_child(t *testing.T) {
	// 1
	root_heading := heading{
		level:  1,
		levels: [6]int{1, 0, 0, 0, 0, 0},
	}
	child_heading := heading{
		level:  4,
		levels: [6]int{1, 0, 0, 1, 0, 0},
	}

	if !is_direct_child(root_heading, child_heading) {
		t.Fatalf("child_heading should be a direct child of root_heading.\n"+
			"root_heading:\n%+v\nchild_heading:\n%+v\n", root_heading, child_heading)
	}

	// 2
	root_heading = heading{
		level:  1,
		levels: [6]int{1, 0, 0, 0, 0, 0},
	}
	child_heading = heading{
		level:  6,
		levels: [6]int{1, 0, 0, 1, 0, 1},
	}

	if is_direct_child(root_heading, child_heading) {
		t.Fatalf("child_heading should NOT be a direct child of root_heading.\n"+
			"root_heading:\n%+v\nchild_heading:\n%+v\n", root_heading, child_heading)
	}

	// 3 - two root headings
	root_heading = heading{
		level:  1,
		levels: [6]int{1, 0, 0, 0, 0, 0},
	}
	child_heading = heading{
		level:  1,
		levels: [6]int{2, 0, 0, 0, 0, 0},
	}

	if is_direct_child(root_heading, child_heading) {
		t.Fatalf("child_heading should NOT be a direct child of root_heading.\n"+
			"root_heading:\n%+v\nchild_heading:\n%+v\n", root_heading, child_heading)
	}
}

func Test_print_tree(t *testing.T) {
	// 1
	test_buffer := bytes.NewBuffer([]byte(""))

	expected_result := "test.md\n" +
		"`-- 1. # test markdown doc (1)\n" +
		"    |-- 1.1. ## header2 (3)\n" +
		"    |   |-- 1.1.1. #### header4 (5)\n" +
		"    |   |-- 1.1.2. #### another header4 (13)\n" +
		"    |   |-- 1.1.3. ####third header4 (17)\n" +
		"    |   `-- 1.1.4. ####fourth #header4 (19)\n" +
		"    |       `-- 1.1.4.1. ##### header5 (23)\n" +
		"    `-- 1.2. ## a second header2 (25)\n" +
		"        `-- 1.2.1. #### testing (27)\n"

	print_tree("test_files/test.md", test_buffer)

	if test_buffer.String() != expected_result {
		t.Fatalf("test_buffer.String() and expected result differ!\n"+
			"test_buffer.String():\n%s\nexpected_result:\n%s\n",
			test_buffer.String(), expected_result)
	}

	// 2 - crlf line endings
	test_buffer.Reset()

	expected_result = "crlf.md\n" +
		"`-- 1. # test markdown doc (1)\n" +
		"    |-- 1.1. ## header2 (3)\n" +
		"    |   |-- 1.1.1. #### header4 (5)\n" +
		"    |   |-- 1.1.2. #### another header4 (13)\n" +
		"    |   |-- 1.1.3. ####third header4 (17)\n" +
		"    |   `-- 1.1.4. ####fourth #header4 (19)\n" +
		"    |       `-- 1.1.4.1. ##### header5 (23)\n" +
		"    `-- 1.2. ## a second header2 (25)\n" +
		"        `-- 1.2.1. #### testing (27)\n"

	print_tree("test_files/crlf.md", test_buffer)

	if test_buffer.String() != expected_result {
		t.Fatalf("test_buffer.String() and expected result differ!\n"+
			"test_buffer.String():\n%q\nexpected_result:\n%q\n",
			test_buffer.String(), expected_result)
	}

	// 3 - big file audio.md
	test_buffer.Reset()

	expected_result = "audio.md\n" +
		"|-- 1. ## alsa (1)\n" +
		"|   |-- 1.1. #### aplay (8)\n" +
		"|   |-- 1.2. #### configuration (59)\n" +
		"|   |-- 1.3. #### select default soundcard and/or pcm device (85)\n" +
		"|   `-- 1.4. #### audioservers (109)\n" +
		"|-- 2. ## how to figure out if you are using pulseaudio or pipewire? (114)\n" +
		"|-- 3. ## pipewire (128)\n" +
		"|   |-- 3.1. #### pipewire - record audio (137)\n" +
		"|   `-- 3.2. #### pipe audio to pipewire (144)\n" +
		"|-- 4. ## pulseaudio (151)\n" +
		"|   |-- 4.1. #### installation (153)\n" +
		"|   |-- 4.2. #### pulseaudio commands (161)\n" +
		"|   |-- 4.3. #### testing pulseaudio (171)\n" +
		"|   |-- 4.4. #### ffmpeg (188)\n" +
		"|   |-- 4.5. #### play sounds on the command line (204)\n" +
		"|   `-- 4.6. #### test microphone (219)\n" +
		"|-- 5. ## Volume problem in movies (239)\n" +
		"|   |-- 5.1. #### audacity (312)\n" +
		"|   `-- 5.2. #### location of sound files that applications use (326)\n" +
		"`-- 6. ## troubleshooting (346)\n" +
		"    |-- 6.1. #### no audio input from headset (348)\n" +
		"    `-- 6.2. #### no sound issue on Ubuntu (359)\n"

	print_tree("test_files/audio.md", test_buffer)

	if test_buffer.String() != expected_result {
		t.Fatalf("test_buffer.String() and expected result differ!\n"+
			"test_buffer.String():\n%q\nexpected_result:\n%q\n",
			test_buffer.String(), expected_result)
	}

	// 4 - yamlfmt.md
	test_buffer.Reset()

	expected_result = "yamlfmt.md\n" +
		"|-- 1. # yamlfmt (1)\n" +
		"|   |-- 1.1. ## Goals (5)\n" +
		"|   |-- 1.2. ## Maintainers (11)\n" +
		"|   |-- 1.3. ## Blog (15)\n" +
		"|   |-- 1.4. ## Installation (21)\n" +
		"|   `-- 1.5. ## Basic Usage (35)\n" +
		"`-- 2. # Configuration File (54)\n"

	print_tree("test_files/yamlfmt.md", test_buffer)

	if test_buffer.String() != expected_result {
		t.Fatalf("test_buffer.String() and expected result differ!\n"+
			"test_buffer.String():\n%q\nexpected_result:\n%q\n",
			test_buffer.String(), expected_result)
	}

	// 5 - badly formatted md document with out of tree headers
	// this should not break completely as every sequence of chars is valid markdown
	test_buffer.Reset()

	expected_result = "weird_headers.md\n" +
		"|-- 1. #### test markdown doc (3)\n" +
		"|-- 1. ## header2 (5)\n" +
		"|   |-- 1.1. #### header4 (7)\n" +
		"|   |-- 1.2. #### another header4 (15)\n" +
		"|   |-- 1.3. ####third header4 (19)\n" +
		"|   `-- 1.4. ####fourth #header4 (21)\n" +
		"|       `-- 1.4.1. ##### header5 (25)\n" +
		"|-- 2. ## a second header2 (27)\n" +
		"|   `-- 2.1. #### testing (29)\n" +
		"`-- 1. # another weird out of tree header (35)\n"

	print_tree("test_files/weird_headers.md", test_buffer)

	if test_buffer.String() != expected_result {
		t.Fatalf("test_buffer.String() and expected result differ!\n"+
			"test_buffer.String():\n%q\nexpected_result:\n%q\n",
			test_buffer.String(), expected_result)
	}
}

func Benchmark_print_tree(b *testing.B) {
	for i := 0; i < b.N; i++ {
		print_tree("test_files/audio.md", bytes.NewBuffer([]byte("")))
	}
}
