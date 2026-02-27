#include <stdio.h>

enum TokenType {
    INT, IDENTIFIER, EQUAL, SEMICOLON, INT_VALUE
};

enum State {
    START, IN_INTEGER, IN_IDENTIFIER, ACCEPT
};

typedef enum Token Token;

typedef enum State State;

/*
 * struct:
 *    type,
 *    værdi (enten string eller int) (hint: brug union)
 *
 * struct:
 *    input (vores kode string),
 *    position (hvilket char er vi ved)
 * 
 * Lexer new_lexer(char* input)
 *    Lav ny lexer of retuner den
 *
 * Token next_token(Lexer lexer)
 *    loop (stop loopet ved ACCEPT state)
 *
 *        if state is START and char == '='
 *            return Token(TokenType.EQUAL)
 *        if state is START and char == ';'
 *            return ...
 *
 *
 *        if state is START and if a-z | A-Z | _
 *            set state IN_IDENTIFIER
 *
 *        if state is IN_IDENTIFIER and if a-z | A-Z | _
 *            set state IN_IDENTIFIER
 *
 *        if state is IN_IDENTIFIER
 *            set state ACCEPT
 *
 *
 *        if state is START and if 0-9
 *            set state IN_INTEGER
 *
 *        if state is IN_INTEGER and if 0-9
 *            set state IN_INTEGER
 *
 *        if state is IN_INTEGER
 *            set state ACCEPT
 *
 *    switch på Identifier'en
 *        hvis den matcher et keyword (som 'int'),
 *              så lav den til et keyword i stedet.
 */

int main() {
    printf("Hello, World!\n");
    char *str = "int x = 10;";
    printf("%s", str);

    return 0;
}
