
/* Example:

Input: [int, identifier, equal, string, semicolon]

Output: 
Program(
    VarDeclaration(
        name: x
        type: string
        value: "abc"
    )
    Print(
        "Hello World!"
    )
    IfStatement(
        condition: 5 > 4
        Block(
            Print(
                "Hello again."
            )
        )
    )
)
*/

/* Grammar:

Program ::= statement*

statement ::= IfStatement | Print | VarDeclaration

IfStatement ::= "if" "(" expression ")" "{" statement* "}"

Print ::= "print" (" expression ")" ";"

VarDeclaration ::= (int | str) Identifier "=" expression ";"

expression ::= comparison ('||' | '&&' comparison)*

comparison ::= term ('*', '/' term)*

term ::= factor ('+' | '-' factor)*

factor ::= int | str | identifier
*/

// IntType, ... IntValue, ...
// int x = 10;

typedef enum {
    IF,
    Print,
    Plus,
    Minus,
    Multiplication,
    Division,
    Equal,
    EqualEqual,
    StringType,
    IntType,
    StringValue,
    IntValue,
} TokenType;

typedef struct {
    TokenType type;
    union {
        int int_value;
        char *string_value;
    } value;
} Token;




int main() {

    return 0;
}