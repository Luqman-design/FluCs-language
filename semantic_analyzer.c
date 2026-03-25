#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"

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
  int a
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
#define MAX_SCOPE 100

typedef struct {
  char name[32];
  TokenType type;
  UT_hash_handle hh;
} VariableEntry;

VariableEntry *states[MAX_SCOPE];
int state_top = -1;

void enter_state() {
  state_top++;
  states[state_top] = NULL;
}

void exit_state()
{
  VariableEntry *current, *tmp;

  HASH_ITER(hh, states[state_top], current, tmp)
  {
    HASH_DEL(states[state_top], current);
    free(current);
  }

  state_top--;
}

void insert_variable(const char *name, TokenType type) {
 
  VariableEntry *v;
  HASH_FIND_STR(states[state_top], name, v);

  if (v != NULL) {
    printf("Semantic error: Variable %s is already declared", name);
    exit(1);
  }
  v = malloc(sizeof(VariableEntry));
  strcopy(v->name, name);
  v->type = type;

  HASH_ADD_STR(states[state_top], name, v);
}

VariableEntry *lookup_variable(const char *name) {
  // Loops through each state, starting from the leaf/top
  for (int i = state_top; i >= 0; i--) {
    VariableEntry *v;
    HASH_FIND_STR(states[i], name, v);
      if (v)
      return v;
  }

  return NULL;
}


/* One struct = one hashmap entry
typedef struct {
    char key[32];
    int  value;
    UT_hash_handle hh;  // makes this struct hashable
} Item;

int main(void) {
    / Dynamic array of hashmap pointers /
    int n = 2;
    Item **maps = calloc(n, sizeof(Item));  // [{}, {}]

    * Fill map[0] /
    Iteme = malloc(sizeof(Item));
    strcpy(e->key, "score");  e->value = 42;
    HASH_ADD_STR(maps[0], key, e);

    * Fill map[1] /
    e = malloc(sizeof(Item));
    strcpy(e->key, "score");  e->value = 99;
    HASH_ADD_STR(maps[1], key, e);

    / Read back /
    for (int i = 0; i < n; i++) {
        Itemfound;
  HASH_FIND_STR(maps[i], "score", found);
        if (found) printf("map[%d] score = %d\n", i, found->value);
    }

    return 0;
}

*/