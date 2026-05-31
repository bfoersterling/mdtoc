#!/usr/bin/bash

function assert_str_eq() {
	test_no="$1"
	s1="$2"
	s2="$3"

	if [[ "$s1" != "$s2" ]]; then
		echo "Test ${test_no} failed."
		echo "s1: \"${s1}\" and s2: \"${s2}\" differ."
		exit 1
	else
		echo "Test ${test_no} passed."
	fi
}

printf "Integration tests for mdtoc.\n\n"

make -C ..

printf "\nIntegration tests start.\n"

# 1
output_1=$(echo "# foo" | ../mdtoc)
expected_1="1. foo (1)"

assert_str_eq "1" "$output_1" "$expected_1"

printf "Integration tests end.\n"

exit 0
