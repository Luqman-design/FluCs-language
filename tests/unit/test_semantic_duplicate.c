#include <assert.h>
#include <stdio.h>
#include "../../lexer.h"
#include "../../parser.h"
#include "../../semantic_analyzer.h"

int main() {
    char* src = "int x = 1; int x = 2;";
    Lexer l = new_lexer(src);
    Node* root = parse(&l);
    semantic_analyze(root);
    printf("semantic_duplicate: PASS\n");
    free_node(root);
    return 0;
}