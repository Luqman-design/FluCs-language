#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	INT, 
	IDENTIFIER,
	EQUAL,
	SEMICOLON,
	INT_VALUE
} TokenType;

typedef enum
{
	START,
	IN_INTEGER,
	IN_IDENTIFIER,
	ACCEPT
} State;

typedef struct
{
	TokenType type;
	union
	{
		char *string_value;
		int int_value;
	} value;
} Token;

typedef struct
{
	char *input;
	int position;
} Lexer;

Lexer new_lexer(char *input)
{
	Lexer lexer;
	lexer.input = input;
	lexer.position = 0;
	return lexer;
}

// return the current character 
char peek(Lexer *lexer)
{
	return lexer->input[lexer->position];
}

Token next_token(Lexer *lexer)
{
	State state = START;
	char temp_token[64]; 
	int temp_token_tracker = 0;

	while (state != ACCEPT)
	{
		char current_char = peek(lexer);
		if (state == START && current_char == '=')
		{
			Token token;
			token.type = EQUAL;
			lexer->position++;
			return token;
		}
		else if (state == START && current_char == ';')
		{
			Token token;
			token.type = SEMICOLON;
			lexer->position++;
			return token;
		}
		else if (state == START && (current_char == '_' || isalpha(current_char)))
		{
			temp_token[temp_token_tracker] = current_char;
			temp_token_tracker++;
			lexer->position++;
			state = IN_IDENTIFIER;
		}
		else if (state == IN_IDENTIFIER && (current_char == '_' || isalpha(current_char)))
		{
			temp_token[temp_token_tracker] = current_char;
			temp_token_tracker++;
			lexer->position++;
		}
		else if (state == IN_IDENTIFIER)
		{
			temp_token[temp_token_tracker] = '\0';
			lexer->position++;

			Token token;
			token.type = IDENTIFIER;
			token.value.string_value = temp_token;

			printf("Token: %s\n", temp_token);
			
			if (strcmp(temp_token, "int") == 0)
			{
				token.type = INT; 
			}
			printf("Token type: %d\n", token.type);
			return token;
		}
		else if (state == START && isdigit(current_char))
		{
			temp_token[temp_token_tracker] = current_char;
			temp_token_tracker++;
			lexer->position++;
			state = IN_INTEGER;
		}
		else if (state == IN_INTEGER && isdigit(current_char))
		{
			temp_token[temp_token_tracker] = current_char;
			temp_token_tracker++;
			lexer->position++;
		}
		else if (state == IN_INTEGER)
		{
			temp_token_tracker++;
			temp_token[temp_token_tracker] = '\0';
			lexer->position++;

			Token token;
			token.type = INT_VALUE;
			token.value.int_value = atoi(temp_token); // convert string to int
			return token;
		}
	}
	Token token;
	return token;
}

int main()
{
	char *str = "int x = 10;\n";
	printf("%s", str);
	Lexer lexer = new_lexer(str);
	Token token = next_token(&lexer);
	Token token2 = next_token(&lexer);

	return 0;
}
