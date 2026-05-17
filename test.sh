#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

if [ ! -f codegen ]; then
    echo "Building codegen..."
    gcc -o codegen codegen.c lexer.c parser.c semantic_analyzer.c -Wall -Wextra
fi

PASS=0
FAIL=0
TOTAL=0

run_test() {
    local file="$1"
    local expected="$2"
    local desc="$3"
    local name="${file%.flucs}"

    TOTAL=$((TOTAL + 1))
    output=$(./codegen "$file" 2>&1)
    actual=$(echo "$output" | head -1)
    errors=$(echo "$output" | grep -c "error:" || true)

    if [ "$errors" -gt 0 ]; then
        printf "  %-10s %-40s FAIL (C compile error)\n" "$name" "$desc"
        FAIL=$((FAIL + 1))
    elif [ "$actual" = "$expected" ]; then
        printf "  %-10s %-40s PASS\n" "$name" "$desc"
        PASS=$((PASS + 1))
    else
        printf "  %-10s %-40s FAIL (expected '%s', got '%s')\n" "$name" "$desc" "$expected" "$actual"
        FAIL=$((FAIL + 1))
    fi
}

echo "Running FluCs test suite..."
echo
echo "  Test       Description                                  Result"
echo "  ----       -----------                                  ------"

run_test "case1.flucs"  "11"    "thread void func"
run_test "case2.flucs"  "1133"  "thread int func + await"
run_test "case3.flucs"  "0"     "thread=N for loop"
run_test "case4.flucs"  "44"    "thread void + shared var"
run_test "case5.flucs"  "1133"  "process int func + await"
run_test "case6.flucs"  "30"    "thread { } block"
run_test "case7.flucs"  "11987" "threaded for loop in process"
run_test "case8.flucs"  "1111"  "thread in if block"
run_test "case9.flucs"  "38"    "process in if block"
run_test "case10.flucs" "60"    "thread in for loop"
run_test "case11.flucs" "1030"  "thread inside function body"
run_test "case12.flucs" "30"    "thread { } block in if"
run_test "case13.flucs" "1133"  "thread + process in separate ifs"
run_test "case14.flucs" "45"    "process in for loop"
run_test "case15.flucs" "15300" "thread void func in if"
run_test "case16.flucs" "20"    "thread in nested if"
run_test "case17.flucs" "70"    "thread in else branch"
run_test "case18.flucs" "30"    "process with local var args"
run_test "case19.flucs" "42"    "process in if with local vars"

echo
echo "Results: $PASS passed, $FAIL failed, $TOTAL total"

if [ "$FAIL" -gt 0 ]; then
    exit 1
fi
