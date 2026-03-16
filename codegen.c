

/*

function emit_statement(Node, output) {
    if Node == IfStatement {
        output += "
        if (Node.expression) {
            emit_block(Node.then_block)
        }
        "
        if Node.else_branch is not null {
            ...
        }
    }
}

function emit_program(Node) {
  if Node == ProgramNode {
      output += "
          #include<stdlib.h>

          int main() {
          "

      // parse program
      for each statement in Node {
          output += emit_statement(statement, output)
      }

      output += "
          return 0;
          }
          "
  }
}

function codegen {

  tokens = lexer(input)

  AST/Node = parser(tokens)

  output = ""
  emit_program(AST/Node, output)

  output til .c file
  run "TCC file.c -o main"
  run "./main"

}

*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

void emit_statement(Node *node, char **output) {
	
}

void emit_program(Node *node, char **output, int *output_length) {
	if (node.type == NODE_PROGRAM) {
		char *tmp += "#include <stdlib.h>\
				   int main() {\0";
		
	    if (strlen(tmp) + strlen(output) > output_length) {
	    	
	    }
	}
	
}

int main() {
  char *str = "if(a < b){print(x);}\n";
  int *output_length = 30;
  char *output = (char *) malloc((output_length + 1) * sizeof(char));

  Lexer lexer = new_lexer(str);

  Node *root = parse(&lexer);

  emit_program(root, &output, &output_length);

  return 0;
}
