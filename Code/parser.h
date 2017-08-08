// Variables a utilizar en el parser.h

token current_token;
token temp_token;
token *current_token_ptr;
token *temp_token_ptr;

// Funciones

token next_token(void);

void match(token tok);

void syntax_error(token tok);

void id_list(void);

void add_op(void);

void primary(void);

void expression(void);

void expr_list(void);

void statement(void);

void statement_list(void);

void program(void);

void system_goal(void);

void read_file(void);
