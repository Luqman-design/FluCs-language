#include <assert.h>
#include <stdio.h>
#include "../../parser.h"
#include "../../lexer.h"

int main() {
    char* src = "int x = 1;";
    Lexer l = new_lexer(src);
    Node* n = parse(&l);
    assert(n->type == NODE_PROGRAM);
    printf("parser_unit: PASS\n");
    free_node(n);
    return 0;
}