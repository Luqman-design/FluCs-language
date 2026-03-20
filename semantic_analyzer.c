#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Case 1:
string a--- program ---a
            /      \
          if       if ---- a
         /  \     /  \
      int a  a  int a \
                    float a

Case 2:
string a,
string b,
if {
  int a,
  a = 10
  b = "gg"
}
if {
  string a = "abc"
}
if {
  a = "test"
},
a

Solution steps to case 2:
steps:
1. global scope
[
  scope 0:
  { (a: str), (b: str) }
]

2. enters if
[
  scope 0:
  { (a: str), (b: str) },

  scope 1: <- lookup here first, the lookup parent scopes.
  { (a: int) }
]

3. exits if
[
  scope 0:
  { (a: str), (b: str) }
]

*/

void semantic_analysis(Node *root) {}
