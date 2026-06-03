#!/usr/bin/bash

function assert_int_eq() {
	declare test_no
	declare int1
	declare int2
	test_no="$1"
	int1="$2"
	int2="$3"

	if [[ "$int1" -ne "$int2" ]]; then
		printf "Test %s failed.\n" "$test_no"
		printf "Ints differ.\n"
		printf "int1: %d.\n" "$int1"
		printf "int2: %d.\n" "$int2"
		exit 1
	else
		printf "Test %s passed.\n" "$test_no"
	fi
}

function assert_rc_eq() {
	declare rc="$?"
	declare test_no="$1"
	declare expected_rc="$2"

	if [[ "$rc" == "$expected_rc" ]]; then
		printf "Test %s passed.\n" "$test_no"
	else
		printf "Test %s failed.\n" "$test_no"
		printf "Actual return code: %d\n" "$rc"
		printf "Expected return code: %d\n" "$expected_rc"

		exit 1
	fi
}

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
		printf "Test %s passed.\n" "$test_no"
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

# 4
output_4=$("$MDTOC" -c 2.1 "${SCRIPT_DIR}/input/basic.md")
expected_4=$'#### sub bar\n\nBar.\n\n```\necho "hello"\n```\n\nMore text.'

assert_str_eq "4" "$output_4" "$expected_4"

# 5 - passing a dir should not segfault (exit code 139), but exit with rc 1.
"$MDTOC" "$(mktemp -d)" &>/dev/null

assert_int_eq "5" "$?" "1"

# 6 - passing a dir with interactive mode
"$MDTOC" -i "$(mktemp -d)" &>/dev/null

assert_rc_eq "6" "1"

printf "Integration tests end.\n"

exit 0
