#include <assert.h>
#include <stdio.h>
#include "../../semantic_analyzer.h"
#include "../../parser.h"
#include "../../lexer.h"

int main() {
    char* src = "int x = 1; print(x);";
    Lexer l = new_lexer(src);
    Node* root = parse(&l);
    semantic_analyze(root);
    printf("semantic_unit: PASS\n");
    free_node(root);
    return 0;
}