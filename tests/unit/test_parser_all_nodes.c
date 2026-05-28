#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../../parser.h"
#include "../../lexer.h"

static Node *parse_src(const char *src) {
    Lexer l = new_lexer((char *)src);
    return parse(&l);
}

static void test_var_decl_int(void) {
    Node *n = parse_src("int x = 42;");
    assert(n->type == NODE_PROGRAM);
    assert(n->body.program.statement_count == 1);
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_VAR_DECLARATION);
    assert(stmt->body.var_declaration.variable_type == TOKEN_INT_TYPE);
    assert(strcmp(stmt->body.var_declaration.variable_name, "x") == 0);
    assert(stmt->body.var_declaration.variable_value->type == NODE_INT_VALUE);
    assert(stmt->body.var_declaration.variable_value->body.int_value.value == 42);
    free_node(n);
    printf("  test_var_decl_int: PASS\n");
}

static void test_var_decl_string(void) {
    Node *n = parse_src("string s = \"hi\";");
    assert(n->body.program.statement_count == 1);
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_VAR_DECLARATION);
    assert(stmt->body.var_declaration.variable_type == TOKEN_STRING_TYPE);
    assert(stmt->body.var_declaration.variable_value->type == NODE_STRING_VALUE);
    free_node(n);
    printf("  test_var_decl_string: PASS\n");
}

static void test_var_update_assign(void) {
    Node *n = parse_src("int x = 1; x = 2;");
    assert(n->body.program.statement_count == 2);
    Node *up = n->body.program.statements[1];
    assert(up->type == NODE_VAR_UPDATE);
    assert(strcmp(up->body.var_update.variable_name, "x") == 0);
    assert(up->body.var_update.operator_type == TOKEN_EQUAL);
    free_node(n);
    printf("  test_var_update_assign: PASS\n");
}

static void test_var_update_plus_equal(void) {
    Node *n = parse_src("int x = 1; x += 5;");
    Node *up = n->body.program.statements[1];
    assert(up->type == NODE_VAR_UPDATE);
    assert(up->body.var_update.operator_type == TOKEN_PLUS_EQUAL);
    free_node(n);
    printf("  test_var_update_plus_equal: PASS\n");
}

static void test_var_update_minus_equal(void) {
    Node *n = parse_src("int x = 10; x -= 3;");
    Node *up = n->body.program.statements[1];
    assert(up->type == NODE_VAR_UPDATE);
    assert(up->body.var_update.operator_type == TOKEN_MINUS_EQUAL);
    free_node(n);
    printf("  test_var_update_minus_equal: PASS\n");
}

static void test_var_update_plus_plus(void) {
    Node *n = parse_src("int i = 0; i++;");
    Node *up = n->body.program.statements[1];
    assert(up->type == NODE_VAR_UPDATE);
    assert(up->body.var_update.operator_type == TOKEN_PLUS_PLUS);
    assert(up->body.var_update.value == NULL);
    free_node(n);
    printf("  test_var_update_plus_plus: PASS\n");
}

static void test_print_int(void) {
    Node *n = parse_src("print(1);");
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_PRINT);
    assert(stmt->body.print.print_value->type == NODE_INT_VALUE);
    free_node(n);
    printf("  test_print_int: PASS\n");
}

static void test_print_identifier(void) {
    Node *n = parse_src("int y = 5; print(y);");
    Node *stmt = n->body.program.statements[1];
    assert(stmt->type == NODE_PRINT);
    assert(stmt->body.print.print_value->type == NODE_IDENTIFIER);
    free_node(n);
    printf("  test_print_identifier: PASS\n");
}

static void test_if_no_else(void) {
    Node *n = parse_src("if (1) { int x = 0; }");
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_IF_STATEMENT);
    assert(stmt->body.if_statement.else_branch == NULL);
    assert(stmt->body.if_statement.then_branch->type == NODE_BLOCK);
    free_node(n);
    printf("  test_if_no_else: PASS\n");
}

static void test_if_with_else(void) {
    Node *n = parse_src("if (1) { int a = 1; } else { int b = 2; }");
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_IF_STATEMENT);
    assert(stmt->body.if_statement.else_branch != NULL);
    free_node(n);
    printf("  test_if_with_else: PASS\n");
}

static void test_if_else_if(void) {
    Node *n = parse_src("if (1) { int a = 1; } else if (2) { int b = 2; }");
    Node *stmt = n->body.program.statements[0];
    assert(stmt->body.if_statement.else_branch != NULL);
    assert(stmt->body.if_statement.else_branch->type == NODE_IF_STATEMENT);
    free_node(n);
    printf("  test_if_else_if: PASS\n");
}

static void test_for_loop(void) {
    Node *n = parse_src("for (int i = 0; i < 10; i++) { print(i); }");
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_FOR_LOOP);
    assert(stmt->body.for_loop.type == PARALLEL_TYPE_REGULAR);
    assert(stmt->body.for_loop.initializer != NULL);
    assert(stmt->body.for_loop.condition != NULL);
    assert(stmt->body.for_loop.updater != NULL);
    assert(stmt->body.for_loop.body != NULL);
    free_node(n);
    printf("  test_for_loop: PASS\n");
}

static void test_function_void_no_params(void) {
    Node *n = parse_src("func void hello() { print(1); }");
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_FUNCTION);
    assert(stmt->body.function.return_type == TOKEN_VOID);
    assert(strcmp(stmt->body.function.name, "hello") == 0);
    assert(stmt->body.function.param_count == 0);
    assert(stmt->body.function.statement_count == 1);
    free_node(n);
    printf("  test_function_void_no_params: PASS\n");
}

static void test_function_int_params(void) {
    Node *n = parse_src("func int add(int a, int b) { return a + b; }");
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_FUNCTION);
    assert(stmt->body.function.return_type == TOKEN_INT_TYPE);
    assert(stmt->body.function.param_count == 2);
    assert(stmt->body.function.params[0].type == TOKEN_INT_TYPE);
    assert(strcmp(stmt->body.function.params[0].name, "a") == 0);
    free_node(n);
    printf("  test_function_int_params: PASS\n");
}

static void test_return_statement(void) {
    Node *n = parse_src("func int id(int x) { return x; }");
    Node *fn = n->body.program.statements[0];
    Node *ret = fn->body.function.statements[0];
    assert(ret->type == NODE_RETURN_STATEMENT);
    assert(ret->body.return_statement.expression->type == NODE_IDENTIFIER);
    free_node(n);
    printf("  test_return_statement: PASS\n");
}

static void test_binary_expression_arithmetic(void) {
    Node *n = parse_src("int r = 3 + 4 * 2;");
    Node *decl = n->body.program.statements[0];
    assert(decl->body.var_declaration.variable_value->type == NODE_BINARY_OPERATION);
    free_node(n);
    printf("  test_binary_expression_arithmetic: PASS\n");
}

static void test_binary_expression_comparison(void) {
    Node *n = parse_src("int r = 1 == 1;");
    Node *decl = n->body.program.statements[0];
    assert(decl->body.var_declaration.variable_value->type == NODE_BINARY_OPERATION);
    assert(decl->body.var_declaration.variable_value->body.binary_operation.operator_type == TOKEN_EQUAL_EQUAL);
    free_node(n);
    printf("  test_binary_expression_comparison: PASS\n");
}

static void test_thread_block(void) {
    Node *n = parse_src("thread { print(1); }");
    Node *stmt = n->body.program.statements[0];
    assert(stmt->type == NODE_THREAD);
    assert(stmt->body.thread.statement_count == 1);
    free_node(n);
    printf("  test_thread_block: PASS\n");
}

static void test_function_call_regular(void) {
    /* Regular (non-parallel) call only appears as the RHS of a var declaration
       or as a thread/process call statement. Here we test it via thread. */
    Node *n = parse_src("func void foo() { print(1); } thread foo();");
    assert(n->body.program.statement_count == 2);
    Node *call = n->body.program.statements[1];
    assert(call->type == NODE_FUNCTION_CALL);
    assert(call->body.function_call.type == PARALLEL_TYPE_THREAD);
    assert(strcmp(call->body.function_call.name, "foo") == 0);
    free_node(n);
    printf("  test_function_call_regular: PASS\n");
}

static void test_function_call_thread(void) {
    Node *n = parse_src("func void bar() { print(2); } thread bar();");
    Node *call = n->body.program.statements[1];
    assert(call->type == NODE_FUNCTION_CALL);
    assert(call->body.function_call.type == PARALLEL_TYPE_THREAD);
    assert(strcmp(call->body.function_call.name, "bar") == 0);
    free_node(n);
    printf("  test_function_call_thread: PASS\n");
}

static void test_await(void) {
    Node *n = parse_src("int r = 0; await { r };");
    Node *stmt = n->body.program.statements[1];
    assert(stmt->type == NODE_AWAIT);
    free_node(n);
    printf("  test_await: PASS\n");
}

static void test_multiple_statements(void) {
    Node *n = parse_src("int a = 1; int b = 2; int c = 3;");
    assert(n->body.program.statement_count == 3);
    free_node(n);
    printf("  test_multiple_statements: PASS\n");
}

static void test_nested_if_in_for(void) {
    Node *n = parse_src("for (int i = 0; i < 5; i++) { if (i) { print(i); } }");
    Node *loop = n->body.program.statements[0];
    assert(loop->type == NODE_FOR_LOOP);
    Node *body = loop->body.for_loop.body;
    assert(body->type == NODE_BLOCK);
    assert(body->body.block.statements[0]->type == NODE_IF_STATEMENT);
    free_node(n);
    printf("  test_nested_if_in_for: PASS\n");
}

int main(void) {
    printf("=== test_parser_all_nodes ===\n");
    test_var_decl_int();
    test_var_decl_string();
    test_var_update_assign();
    test_var_update_plus_equal();
    test_var_update_minus_equal();
    test_var_update_plus_plus();
    test_print_int();
    test_print_identifier();
    test_if_no_else();
    test_if_with_else();
    test_if_else_if();
    test_for_loop();
    test_function_void_no_params();
    test_function_int_params();
    test_return_statement();
    test_binary_expression_arithmetic();
    test_binary_expression_comparison();
    test_thread_block();
    test_function_call_regular();
    test_function_call_thread();
    test_await();
    test_multiple_statements();
    test_nested_if_in_for();
    printf("test_parser_all_nodes: PASS\n");
    return 0;
}
