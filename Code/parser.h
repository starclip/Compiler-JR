
#include "sem_records.h"
// Variables a utilizar en el parser.h

token current_token;
token temp_token;
token *current_token_ptr;
token *temp_token_ptr;

// Funciones
void ident(expr_rec *id_rec);

token next_token(void);

void match(token tok);

void syntax_error(token tok);

void id_list(void);

void add_op(op_rec *op);

void primary(expr_rec *result);

void expression(expr_rec *result);

void expr_list(void);

void statement(void);

void statement_list(void);

void program(void);

void system_goal(void);

int read_file(void);

void all_comment(void);