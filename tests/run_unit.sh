#!/bin/bash
set -e
gcc -o tests/unit/test_lexer tests/unit/test_lexer.c lexer.c -Wall
gcc -o tests/unit/test_parser tests/unit/test_parser.c lexer.c parser.c -Wall
gcc -o tests/unit/test_semantic tests/unit/test_semantic.c lexer.c parser.c semantic_analyzer.c -Wall
gcc -o tests/unit/test_lexer_negative tests/unit/test_lexer_negative.c lexer.c -Wall
gcc -o tests/unit/test_semantic_type_mismatch tests/unit/test_semantic_type_mismatch.c lexer.c parser.c semantic_analyzer.c -Wall
gcc -o tests/unit/test_semantic_duplicate tests/unit/test_semantic_duplicate.c lexer.c parser.c semantic_analyzer.c -Wall
gcc -o tests/unit/test_lexer_all_tokens tests/unit/test_lexer_all_tokens.c lexer.c -Wall
gcc -o tests/unit/test_parser_all_nodes tests/unit/test_parser_all_nodes.c lexer.c parser.c -Wall
gcc -o tests/unit/test_semantic_all tests/unit/test_semantic_all.c lexer.c parser.c semantic_analyzer.c -Wall
tests/unit/test_lexer && \
tests/unit/test_parser && \
tests/unit/test_semantic && \
tests/unit/test_lexer_negative && \
tests/unit/test_semantic_type_mismatch && \
tests/unit/test_semantic_duplicate && \
tests/unit/test_lexer_all_tokens && \
tests/unit/test_parser_all_nodes && \
tests/unit/test_semantic_all
echo "All unit tests passed"
