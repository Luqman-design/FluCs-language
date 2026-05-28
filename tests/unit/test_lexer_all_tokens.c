#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../../lexer.h"

/* Helper: consume and return next token type */
static TokenType next_type(Lexer *l) {
    return next_token(l).type;
}

static void test_keywords(void) {
    Lexer l = new_lexer("int string void print if else for func return process thread await ");
    assert(next_type(&l) == TOKEN_INT_TYPE);
    assert(next_type(&l) == TOKEN_STRING_TYPE);
    assert(next_type(&l) == TOKEN_VOID);
    assert(next_type(&l) == TOKEN_PRINT);
    assert(next_type(&l) == TOKEN_IF);
    assert(next_type(&l) == TOKEN_ELSE);
    assert(next_type(&l) == TOKEN_FOR);
    assert(next_type(&l) == TOKEN_FUNCTION);
    assert(next_type(&l) == TOKEN_RETURN);
    assert(next_type(&l) == TOKEN_PROCESS);
    assert(next_type(&l) == TOKEN_THREAD);
    assert(next_type(&l) == TOKEN_AWAIT);
    assert(next_type(&l) == TOKEN_EOF);
    printf("  test_keywords: PASS\n");
}

static void test_symbols(void) {
    Lexer l = new_lexer("; ( ) { } * / ,");
    assert(next_type(&l) == TOKEN_SEMICOLON);
    assert(next_type(&l) == TOKEN_LEFT_PAREN);
    assert(next_type(&l) == TOKEN_RIGHT_PAREN);
    assert(next_type(&l) == TOKEN_LEFT_CURLYBRACKET);
    assert(next_type(&l) == TOKEN_RIGHT_CURLYBRACKET);
    assert(next_type(&l) == TOKEN_MULTIPLY);
    assert(next_type(&l) == TOKEN_DIVIDE);
    assert(next_type(&l) == TOKEN_COMMA);
    assert(next_type(&l) == TOKEN_EOF);
    printf("  test_symbols: PASS\n");
}

static void test_operators(void) {
    Lexer l = new_lexer("= == != < > <= >= && || + ++ += - -= ; ");
    assert(next_type(&l) == TOKEN_EQUAL);
    assert(next_type(&l) == TOKEN_EQUAL_EQUAL);
    assert(next_type(&l) == TOKEN_NOT_EQUAL);
    assert(next_type(&l) == TOKEN_LESS);
    assert(next_type(&l) == TOKEN_GREATER);
    assert(next_type(&l) == TOKEN_LESS_EQUAL);
    assert(next_type(&l) == TOKEN_GREATER_EQUAL);
    assert(next_type(&l) == TOKEN_AND);
    assert(next_type(&l) == TOKEN_OR);
    assert(next_type(&l) == TOKEN_PLUS);
    assert(next_type(&l) == TOKEN_PLUS_PLUS);
    assert(next_type(&l) == TOKEN_PLUS_EQUAL);
    assert(next_type(&l) == TOKEN_MINUS);
    assert(next_type(&l) == TOKEN_MINUS_EQUAL);
    printf("  test_operators: PASS\n");
}

static void test_integer_values(void) {
    Lexer l = new_lexer("0 1 42 999 ");
    Token t;
    t = next_token(&l); assert(t.type == TOKEN_INT_VALUE); assert(t.value.int_value == 0);
    t = next_token(&l); assert(t.type == TOKEN_INT_VALUE); assert(t.value.int_value == 1);
    t = next_token(&l); assert(t.type == TOKEN_INT_VALUE); assert(t.value.int_value == 42);
    t = next_token(&l); assert(t.type == TOKEN_INT_VALUE); assert(t.value.int_value == 999);
    assert(next_type(&l) == TOKEN_EOF);
    printf("  test_integer_values: PASS\n");
}

static void test_string_values(void) {
    Lexer l = new_lexer("\"hello\" \"world\"");
    Token t;
    t = next_token(&l);
    assert(t.type == TOKEN_STRING_VALUE);
    assert(strcmp(t.value.string_value, "hello") == 0);
    free_token(&t);
    t = next_token(&l);
    assert(t.type == TOKEN_STRING_VALUE);
    assert(strcmp(t.value.string_value, "world") == 0);
    free_token(&t);
    printf("  test_string_values: PASS\n");
}

static void test_identifier_vs_keyword(void) {
    Lexer l = new_lexer("ints stringa threaded ");
    Token t;
    t = next_token(&l);
    assert(t.type == TOKEN_IDENTIFIER);
    assert(strcmp(t.value.string_value, "ints") == 0);
    free_token(&t);
    t = next_token(&l);
    assert(t.type == TOKEN_IDENTIFIER);
    free_token(&t);
    t = next_token(&l);
    assert(t.type == TOKEN_IDENTIFIER);
    free_token(&t);
    printf("  test_identifier_vs_keyword: PASS\n");
}

static void test_underscore_identifier(void) {
    Lexer l = new_lexer("_foo _bar123 ");
    Token t;
    t = next_token(&l);
    assert(t.type == TOKEN_IDENTIFIER);
    assert(strcmp(t.value.string_value, "_foo") == 0);
    free_token(&t);
    t = next_token(&l);
    assert(t.type == TOKEN_IDENTIFIER);
    free_token(&t);
    printf("  test_underscore_identifier: PASS\n");
}

static void test_whitespace_skipping(void) {
    Lexer l = new_lexer("   \t  int   \n  x  ");
    assert(next_type(&l) == TOKEN_INT_TYPE);
    Token t = next_token(&l);
    assert(t.type == TOKEN_IDENTIFIER);
    assert(strcmp(t.value.string_value, "x") == 0);
    free_token(&t);
    printf("  test_whitespace_skipping: PASS\n");
}

static void test_empty_string(void) {
    Lexer l = new_lexer("\"\"");
    Token t = next_token(&l);
    assert(t.type == TOKEN_STRING_VALUE);
    assert(strcmp(t.value.string_value, "") == 0);
    free_token(&t);
    printf("  test_empty_string: PASS\n");
}

static void test_full_declaration_sequence(void) {
    Lexer l = new_lexer("int foo = 10;");
    assert(next_type(&l) == TOKEN_INT_TYPE);
    Token t = next_token(&l);
    assert(t.type == TOKEN_IDENTIFIER);
    assert(strcmp(t.value.string_value, "foo") == 0);
    free_token(&t);
    assert(next_type(&l) == TOKEN_EQUAL);
    Token v = next_token(&l);
    assert(v.type == TOKEN_INT_VALUE);
    assert(v.value.int_value == 10);
    assert(next_type(&l) == TOKEN_SEMICOLON);
    assert(next_type(&l) == TOKEN_EOF);
    printf("  test_full_declaration_sequence: PASS\n");
}

static void test_not_operator(void) {
    Lexer l = new_lexer("! ");
    assert(next_type(&l) == TOKEN_NOT);
    printf("  test_not_operator: PASS\n");
}

static void test_eof_on_empty(void) {
    Lexer l = new_lexer("");
    assert(next_type(&l) == TOKEN_EOF);
    printf("  test_eof_on_empty: PASS\n");
}

int main(void) {
    printf("=== test_lexer_all_tokens ===\n");
    test_keywords();
    test_symbols();
    test_operators();
    test_integer_values();
    test_string_values();
    test_identifier_vs_keyword();
    test_underscore_identifier();
    test_whitespace_skipping();
    test_empty_string();
    test_full_declaration_sequence();
    test_not_operator();
    test_eof_on_empty();
    printf("test_lexer_all_tokens: PASS\n");
    return 0;
}
