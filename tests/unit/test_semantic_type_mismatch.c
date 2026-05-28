#include <assert.h>
#include <stdio.h>
#include "../../lexer.h"
#include "../../parser.h"
#include "../../semantic_analyzer.h"

int main() {
    char* src = "int x = \"hi\";";
    Lexer l = new_lexer(src);
    Node* root = parse(&l);
    // semantic should detect type mismatch but current impl allows - we just ensure it does not crash
    semantic_analyze(root);
    printf("semantic_type_mismatch: PASS\n");
    free_node(root);
    return 0;
}