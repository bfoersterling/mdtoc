#!/usr/bin/bash

function assert_str_eq() {
	declare test_no
	test_no="$1"
	declare s1
	declare s2
	s1="$2"
	s2="$3"

	if [[ "$s1" != "$s2" ]]; then
		printf "Test %s failed.\n" "$test_no"
		printf "Strings differ.\n"
		printf "s1:\n\"%q\"\n" "$s1"
		printf "s2:\n\"%q\"\n" "$s2"
		exit 1
	else
		echo "Test ${test_no} passed."
	fi
}

SCRIPT_DIR=$(realpath "$(dirname "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")
MDTOC="${PROJECT_DIR}/mdtoc"

printf "Integration tests for mdtoc.\n\n"

printf "Integration tests start.\n"

# 1
output_1=$(echo "# foo" | "$MDTOC")
expected_1="1. foo (1)"

assert_str_eq "1" "$output_1" "$expected_1"

# 2
output_2=$("$MDTOC" "${SCRIPT_DIR}/input/basic.md")
expected_2=$'1. foo (1)\n'
expected_2+=$'  1.1. sub foo (3)\n'
expected_2+=$'2. bar (10)\n'
expected_2+=$'  2.1. sub bar (12)'

assert_str_eq "2" "$output_2" "$expected_2"

# 3
output_3=$("$MDTOC" "${SCRIPT_DIR}/input/citation.md")
expected_3=$'1. a great quote (1)\n2. and another one (6)'

assert_str_eq "3" "$output_3" "$expected_3"

printf "Integration tests end.\n"

exit 0
