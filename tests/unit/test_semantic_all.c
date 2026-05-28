#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../../lexer.h"
#include "../../parser.h"
#include "../../semantic_analyzer.h"

static void run(const char *src) {
    Lexer l = new_lexer((char *)src);
    Node *root = parse(&l);
    semantic_analyze(root);
    free_node(root);
}

/* ---- simple success cases ---- */

static void test_int_declaration(void) {
    run("int x = 1;");
    printf("  test_int_declaration: PASS\n");
}

static void test_string_declaration(void) {
    run("string s = \"hello\";");
    printf("  test_string_declaration: PASS\n");
}

static void test_multiple_vars(void) {
    run("int a = 1; int b = 2; int c = 3;");
    printf("  test_multiple_vars: PASS\n");
}

static void test_var_update(void) {
    run("int x = 5; x = 10;");
    printf("  test_var_update: PASS\n");
}

static void test_var_update_plus_equal(void) {
    run("int x = 5; x += 3;");
    printf("  test_var_update_plus_equal: PASS\n");
}

static void test_var_update_minus_equal(void) {
    run("int x = 5; x -= 1;");
    printf("  test_var_update_minus_equal: PASS\n");
}

static void test_var_update_plus_plus(void) {
    run("int x = 0; x++;");
    printf("  test_var_update_plus_plus: PASS\n");
}

static void test_print_int_var(void) {
    run("int x = 7; print(x);");
    printf("  test_print_int_var: PASS\n");
}

static void test_print_string_var(void) {
    run("string s = \"yo\"; print(s);");
    printf("  test_print_string_var: PASS\n");
}

static void test_if_statement(void) {
    run("int x = 1; if (x) { print(x); }");
    printf("  test_if_statement: PASS\n");
}

static void test_if_else(void) {
    run("int x = 0; if (x) { int a = 1; } else { int b = 2; }");
    printf("  test_if_else: PASS\n");
}

static void test_for_loop(void) {
    run("for (int i = 0; i < 5; i++) { print(i); }");
    printf("  test_for_loop: PASS\n");
}

static void test_function_void(void) {
    run("func void greet() { print(1); }");
    printf("  test_function_void: PASS\n");
}

static void test_function_int_return(void) {
    run("func int double(int x) { return x + x; }");
    printf("  test_function_int_return: PASS\n");
}

static void test_function_call_regular(void) {
    /* Stand-alone calls must use thread or process prefix; test regular call
       as RHS of a var declaration instead */
    run("func int id(int x) { return x; } int r = id(5);");
    printf("  test_function_call_regular: PASS\n");
}

static void test_arithmetic_expression(void) {
    run("int r = 2 + 3 * 4 - 1;");
    printf("  test_arithmetic_expression: PASS\n");
}

static void test_comparison_expression(void) {
    run("int r = 3 > 2;");
    printf("  test_comparison_expression: PASS\n");
}

static void test_logical_expression(void) {
    run("int r = 1 && 0;");
    printf("  test_logical_expression: PASS\n");
}

static void test_equality_expression(void) {
    run("int r = 5 == 5;");
    printf("  test_equality_expression: PASS\n");
}

static void test_not_equal_expression(void) {
    run("int r = 3 != 4;");
    printf("  test_not_equal_expression: PASS\n");
}

static void test_nested_scope(void) {
    /* variable 'inner' is scoped to the if block, should not leak out */
    run("int x = 1; if (x) { int inner = 2; print(inner); }");
    printf("  test_nested_scope: PASS\n");
}

static void test_thread_block_semantic(void) {
    run("int counter = 0; thread { counter += 10; }");
    /* counter should be marked shared; must not crash */
    printf("  test_thread_block_semantic: PASS\n");
}

static void test_thread_call_semantic(void) {
    run("func void work(int n) { print(n); } thread work(5);");
    printf("  test_thread_call_semantic: PASS\n");
}

static void test_process_call_semantic(void) {
    run("func int double(int x) { return x + x; } int r = process double(3); await { r };");
    printf("  test_process_call_semantic: PASS\n");
}

static void test_await_semantic(void) {
    run("int r = 0; await { r };");
    printf("  test_await_semantic: PASS\n");
}

static void test_shared_var_two_threads(void) {
    /* Two thread blocks accessing the same var: it must be detected as shared
       without crashing. */
    run("int val = 0; thread { val += 1; } thread { val += 2; }");
    Lexer l = new_lexer("int val = 0; thread { val += 1; } thread { val += 2; }");
    Node *root = parse(&l);
    semantic_analyze(root);
    /* Check that val got marked shared */
    Node *decl = root->body.program.statements[0];
    assert(decl->type == NODE_VAR_DECLARATION);
    assert(decl->body.var_declaration.is_shared == 1);
    free_node(root);
    printf("  test_shared_var_two_threads: PASS\n");
}

static void test_function_multiple_params(void) {
    run("func int sum3(int a, int b, int c) { return a + b + c; }");
    printf("  test_function_multiple_params: PASS\n");
}

static void test_string_equality(void) {
    run("string a = \"x\"; string b = \"y\"; int r = a == b;");
    printf("  test_string_equality: PASS\n");
}

int main(void) {
    printf("=== test_semantic_all ===\n");
    test_int_declaration();
    test_string_declaration();
    test_multiple_vars();
    test_var_update();
    test_var_update_plus_equal();
    test_var_update_minus_equal();
    test_var_update_plus_plus();
    test_print_int_var();
    test_print_string_var();
    test_if_statement();
    test_if_else();
    test_for_loop();
    test_function_void();
    test_function_int_return();
    test_function_call_regular();
    test_arithmetic_expression();
    test_comparison_expression();
    test_logical_expression();
    test_equality_expression();
    test_not_equal_expression();
    test_nested_scope();
    test_thread_block_semantic();
    test_thread_call_semantic();
    test_process_call_semantic();
    test_await_semantic();
    test_shared_var_two_threads();
    test_function_multiple_params();
    test_string_equality();
    printf("test_semantic_all: PASS\n");
    return 0;
}
