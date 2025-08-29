package main

import (
	"bytes"
	"testing"
)

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
}

func Benchmark_print_tree(b *testing.B) {
	for i := 0; i < b.N; i++ {
		print_tree("test_files/audio.md", bytes.NewBuffer([]byte("")))
	}
}
