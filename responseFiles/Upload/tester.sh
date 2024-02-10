#!/bin/bash

# Go into the rush directory
cd ..

FILE_CASES=tester/cases
EXECUTABLE=./rush-02

# Display running message
echo "Compiling..."
echo -e "\033[30m---------------------------\033[0m"

# Make the files
make fclean
make

echo ""
echo "Running..."
echo -e "\033[30m---------------------------\033[0m"

# Check if the executable exists and has permissions
if [ ! -x "$EXECUTABLE" ]; then
    echo -e "\033[31mError: Executable '$EXECUTABLE' does not exist or does not have execute permissions.\033[0m"
    exit 1
fi

# Read the input file line by line
cases=()
while IFS= read -r line || [[ -n "$line" ]]; do
    if [[ -n "$line" && ${line:0:1} != "#" ]]; then
        cases+=("$line")
    fi
done < "$FILE_CASES"

# Iterate over the input lines
failures=0
num_lines=${#cases[@]}
for ((i = 0; i < num_lines; i += 2)); do
    input="${cases[i]}"
    expected_output="${cases[i + 1]}"

    std_out=$(eval "$EXECUTABLE $input")
    # std_err=$(eval "$EXECUTABLE $input" 2>&1)
    output="${std_out}${std_err}"

    # output=$(eval "$EXECUTABLE $input")

    # Compare the actual output with the expected output
    if [ "$output" != "$expected_output" ]; then
        fail=true
        echo ""
        echo "Execution: $EXECUTABLE $input"
        echo ""
        echo "Input:    |$input|"
        echo "Output:   |$output|"
        echo "Expected: |$expected_output|"
        echo ""
        echo -e "\033[30m---------------------------\033[0m"
        failures=$((failures + 1))
    fi
done

# Print summary
if [ "$failures" -gt 0 ]; then
    echo -e "\033[31mFAIL: $failures test case(s)\033[0m"
else
    echo -e "\033[32m╔════════════════════════════════════════╗\033[0m"
    echo -e "\033[32m║ Congratiolations you passed all tests! ║\033[0m"
    echo -e "\033[32m╚════════════════════════════════════════╝\033[0m"
fi

echo -e "\033[30m𝓌𝓇𝒾𝓉𝓉𝑒𝓃 𝒷𝓎 𝒻𝒷𝓇𝓊𝑔𝑔𝑒𝓂\033[0m"


