// Constantes
#define LEN_TOKEN_BUFFER  30

// Variables globales
FILE *file;
FILE *file_sData;
FILE *file_sText;
char token_buffer[LEN_TOKEN_BUFFER];
char reserved_words[4][6] = {"begin", "end", "read", "write"};
int pos_token_buffer;

typedef enum token_types{

	BEGIN, END, READ, WRITE, ID, INTLITERAL, 
	LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP,
	PLUSOP, MINUSOP, SCANEOF

} token;


// Funciones

void clear_buffer(void);

void buffer_char(int in_char);

token check_reserved(void);

void lexical_error(int in_char);

token scanner(void);