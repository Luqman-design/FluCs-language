#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../../lexer.h"

int main() {
    Lexer l = new_lexer("int x = 5;");
    Token t = next_token(&l);
    assert(t.type == TOKEN_INT_TYPE);
    t = next_token(&l);
    assert(t.type == TOKEN_IDENTIFIER);
    printf("lexer_unit: PASS\n");
    return 0;
}