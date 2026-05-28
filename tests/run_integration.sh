#!/bin/bash
set -e

if [ ! -f codegen ]; then
  gcc -o codegen codegen.c lexer.c parser.c semantic_analyzer.c -Wall -Wextra
fi

for f in tests/acceptance/case*.flucs; do
  ./codegen "$f" > /dev/null && echo "integration $(basename $f): PASS"
done

if ./codegen tests/integration/invalid_undeclared.flucs > /dev/null 2>&1; then
  echo "integration invalid_undeclared.flucs: FAIL"
  exit 1
fi
echo "integration invalid_undeclared.flucs: PASS"

if ./codegen tests/integration/invalid_duplicate.flucs > /dev/null 2>&1; then
  echo "integration invalid_duplicate.flucs: FAIL"
  exit 1
fi
echo "integration invalid_duplicate.flucs: PASS"

if ./codegen tests/integration/invalid_type.flucs > /dev/null 2>&1; then
  echo "integration invalid_type.flucs: FAIL"
  exit 1
fi
echo "integration invalid_type.flucs: PASS"


