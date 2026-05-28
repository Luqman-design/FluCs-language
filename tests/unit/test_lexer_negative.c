#include <assert.h>
#include <stdio.h>
#include "../../lexer.h"

int main() {
  Lexer l = new_lexer("thread foo();");
  Token t = next_token(&l);
  assert(t.type == TOKEN_THREAD);
  printf("lexer_negative: PASS\n");
  return 0;
}