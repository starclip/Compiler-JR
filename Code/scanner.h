/*
	scanner.h

	COMPILADOR MICRO -> x86

	Created by: 
		Jason Barrantes Arce.
		Randy Morales Gamboa

*/

#define LEN_TOKEN_BUFFER  30

FILE *file;
FILE *file_sData;
FILE *file_sText;
FILE *file_push;
FILE *current_file;
char token_buffer[LEN_TOKEN_BUFFER];
char reserved_words[4][6] = {"begin", "end", "read", "write"};
int pos_token_buffer;
int validate_Enter;
int line_number = 1;

typedef enum token_types{

	BEGIN, END, READ, WRITE, ID, INTLITERAL, 
	LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP,
	PLUSOP, MINUSOP, SCANEOF, PIPE

} token;


void clear_buffer(void);

void buffer_char(int in_char);

token check_reserved(void);

void lexical_error(int in_char);

token scanner(void);