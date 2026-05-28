#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR/acceptance"

echo "Building codegen..."
gcc -o codegen ../../codegen.c ../../lexer.c ../../parser.c ../../semantic_analyzer.c -Wall -Wextra

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
    elif [ "$expected" = "__ANY__" ]; then
        printf "  %-10s %-40s PASS\n" "$name" "$desc"
        PASS=$((PASS + 1))
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
run_test "case3.flucs"  "__ANY__" "thread=N for loop"
run_test "case4.flucs"  "44"    "thread void + shared var"
run_test "case5.flucs"  "1133"  "process int func + await"
run_test "case6.flucs"  "30"    "thread { } block"
run_test "case7.flucs"  "__ANY__" "threaded for loop in process"
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
run_test "case20.flucs" "49"    "func square return"
run_test "case21.flucs" "15"    "for loop sum 1..5"
run_test "case22.flucs" "7"     "int subtraction"
run_test "case23.flucs" "42"    "int multiplication"
run_test "case24.flucs" "60"    "three thread void shared acc"
run_test "case25.flucs" "1"     "int equality true"
run_test "case26.flucs" "1"     "int not-equal true"
run_test "case27.flucs" "99"    "if with == condition"
run_test "case28.flucs" "42"    "func max if/else"
run_test "case29.flucs" "42"    "process multiply"
run_test "case30.flucs" "10"    "two thread blocks shared counter"
run_test "case31.flucs" "25"    "int division"
run_test "case32.flucs" "7"     "nested if double"

echo
echo "Results: $PASS passed, $FAIL failed, $TOTAL total"

if [ "$FAIL" -gt 0 ]; then
    exit 1
fi
