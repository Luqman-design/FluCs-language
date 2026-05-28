#!/bin/bash
set -e

if [ ! -f codegen ]; then
  gcc -o codegen codegen.c lexer.c parser.c semantic_analyzer.c -Wall -Wextra
fi

# ---- valid programs: must exit 0 ----
for f in tests/acceptance/case*.flucs; do
  ./codegen "$f" > /dev/null && echo "integration $(basename $f): PASS"
done

# ---- invalid programs: must exit non-zero ----
expect_fail() {
  local file="$1"
  local desc="$2"
  if ./codegen "$file" > /dev/null 2>&1; then
    echo "integration $(basename $file) ($desc): FAIL (expected error, got success)"
    exit 1
  fi
  echo "integration $(basename $file) ($desc): PASS"
}

expect_fail tests/integration/invalid_undeclared.flucs         "undeclared variable"
expect_fail tests/integration/invalid_duplicate.flucs          "duplicate declaration"
expect_fail tests/integration/invalid_type.flucs               "int = string literal"
expect_fail tests/integration/invalid_redecl_diff_type.flucs   "redeclare with different type"
expect_fail tests/integration/invalid_arith_string.flucs       "arithmetic with string"
expect_fail tests/integration/invalid_use_before_declare.flucs "use before declare"
expect_fail tests/integration/invalid_update_type_mismatch.flucs "update type mismatch"
expect_fail tests/integration/invalid_arith_on_string.flucs    "arith on string var"
