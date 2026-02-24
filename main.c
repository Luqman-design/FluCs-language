#include <stdio.h>

enum Token{
    INT, IDENTIFIER, EQUAL, SEMICOLON, INT_VALUE
};

enum State{
    START, ININTEGER, ACCEPT
};

typedef enum Token Token;

typedef enum State State;


int main() {
    printf("Hello, World!\n");
    char *str = "int x = 10;";
    printf("%s", str);

    return 0;
}