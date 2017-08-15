/*
	parser.c
	COMPILADOR MICRO -> x86

	-El unico tipo de dato es integer
	-Los identificadores son declarados explicitamente, y tienen a lo sumo, 32 caracteres
	-Los unicos literales son hileras de digitos
	-Los comentarios empiezan con -- y terminan con el fin de la linea
	-Solo hay asignaciones y operaciones E/S
	-Todas las instrucciones terminan con ;
	-El programa Micro esta delimitado en un bloque que inicia con begin y termina con end.

	Created by: 
		Jason Barrantes Arce.
		Randy Morales Gamboa
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "scanner.c"
#include "parser.h"
#include "sem_routines.c"

token next_token(void){
	
	if (current_token_ptr == temp_token_ptr || temp_token_ptr == NULL){
		temp_token = scanner();
		temp_token_ptr = &temp_token;
	}

	return temp_token;
}

void match(token tok){

	token tempTok = next_token();
	token *temp;
	//printf("Parameters -> %d, Scanner -> %d\n", tok, tempTok);
	
	if (tempTok == tok){
		current_token = tempTok;
		// *
		temp = current_token_ptr;
		current_token_ptr = &current_token;
		temp_token_ptr = temp;
	}else{

		if(tok==BEGIN || tok==END || tok==SCANEOF){
			syntax_error(tok);
		}else{
			syntax_error(tempTok);
		}
	}
	
	
}


void syntax_error(token tok){	//FALTA HACER

	switch(tok){

		case BEGIN: printf("%s", "Error de sintaxis: BEGIN"); break;
		case END: printf("%s", "Error de sintaxis: END"); break;
		case READ: printf("%s", "Error de sintaxis: READ"); break;
		case WRITE: printf("%s", "Error de sintaxis: WRITE"); break;
		case ID: printf("%s", "Error de sintaxis: ID"); break;
		case INTLITERAL: printf("%s", "Error de sintaxis: INTLITERAL"); break;
		case LPAREN: printf("%s", "Error de sintaxis: LPAREN"); break;
		case RPAREN: printf("%s", "Error de sintaxis: RPAREN"); break;
		case SEMICOLON: printf("%s", "Error de sintaxis: SEMICOLON"); break;
		case COMMA: printf("%s", "Error de sintaxis: COMMA"); break;
		case ASSIGNOP: printf("%s", "Error de sintaxis: ASSIGNOP"); break;
		case PLUSOP: printf("%s", "Error de sintaxis: PLUSOP"); break;
		case MINUSOP: printf("%s", "Error de sintaxis: MINUSOP"); break;
		case SCANEOF: printf("%s", "Error de sintaxis: SCANEOF"); break;
		case PIPE: printf("%s", "Error de sintaxis: PIPE"); break;

		default: break;
	}

	printf(" en la linea: %d\n", line_number);
}

void ident(expr_rec *id_rec){
	match(ID);
	*id_rec = process_id();
}

void id_list(void){		// <id list> ::= ID {  , ID }

	expr_rec id_rec;
	ident(& id_rec);
	read_id(id_rec);

	while(next_token() == COMMA){
		expr_rec id_new;
		match(COMMA);
		ident(& id_new);
		read_id(id_new);
	}
}

void add_op(op_rec *op){		// <add op> ::= PLUSOP | MINUSOP
	
	token tok = next_token();

	if(tok == PLUSOP || tok == MINUSOP){
		match(tok);
		*op = process_op();

	}else{
		syntax_error(tok);
	}
}

void primary(expr_rec *result){

	token tok = next_token();

	switch(tok){

		case LPAREN: 	// <primary> ::= ( <expression> )
			match(LPAREN);
			expression(result);
			match(RPAREN); 
			break;

		case ID:		// <primary> ::= ID
			ident(result);
			break;

		case INTLITERAL:	// <primary> ::= INTLITERAL
			match(INTLITERAL);
			*result = process_literal();
			break;

		default:
			syntax_error(tok);
			break;
	}
}

void expression(expr_rec *result){	// <expression> ::= <primary> { <add op> <primary> }


	expr_rec left_operand, center_operand, right_operand;
	op_rec op;

	primary(&left_operand);	//
	token t = next_token();

	if(t == PIPE){

		match(PIPE);
		primary(&center_operand);

		if(t = next_token() == PIPE){

			match(PIPE);
			primary(&right_operand);

		}else{
			syntax_error(t);
			return;
		}

		left_operand = if_condition(left_operand, center_operand, right_operand);
		*result = left_operand;
		return;
	}


	while(t == PLUSOP || t == MINUSOP){
		add_op(&op);	//
		primary(&right_operand); //
		left_operand = gen_infix(left_operand, op, right_operand);
		t = next_token();
	}

	*result = left_operand;
}

void expr_list(void){		// <expr list> ::= <expression> {  , <expression> }
	
	expr_rec result;
	expression(& result); //QUE PARAMETRO SE LE PASA AQUI?
	write_expr(result);
	
	//printf(" ()0 %d\n", *temp_token_ptr);
	while(next_token() == COMMA){
		expr_rec result;
		match(COMMA);
		expression(& result); //QUE PARAMETRO SE LE PASA AQUI?
		write_expr(result);
	}
}

void statement(void){	//llama a ident ?

	expr_rec er_id;
	expr_rec result;
	/*
		er_id := <expresion>;
	*/
	token tok = next_token();

	switch(tok){
		case ID:		//<statement> ::= ID := <expression> ;
			ident(& er_id);
			match(ASSIGNOP);
			expression(& result);	//QUE PARAMETRO SE LE PASA AQUI?
			match(SEMICOLON);
			assign(er_id, result);
			break;

		case READ:		//<statement> ::= READ ( <id list>) ;
			match(READ);
			match(LPAREN);
			id_list();
			match(RPAREN);
			match(SEMICOLON);
			break;

		case WRITE:		//<statement> ::= WRITE ( <expr list> ) ;
			match(WRITE);
			match(LPAREN);
			expr_list();
			match(RPAREN);
			match(SEMICOLON);
			break;

		default:
			syntax_error(tok);
			break;
	}
}

void statement_list(void){		//<statement list> ::= <statement> { <statement> }

	statement();
	while(0 == 0){
		token tempTok = next_token();
		switch(tempTok){
			case ID:
			case READ:
			case WRITE:
				statement();
				break;
			default: return;
		}
	}
}

void program(void){		//<program> ::= begin <statement list> end

	start();
	match(BEGIN);
	statement_list();
	match(END);
	all_comment(); // Desactiva la búsqueda de palabras. Y toma todo como un comentario.
}

void system_goal(void){		// <system goal> ::= <program> SCANEOF

	program();
	match(SCANEOF);
	finish();
}

int read_file(){

	file = fopen("example_1.txt", "r");
	if (file == NULL){
		printf("%s\n", "El archivo no existe.");
		return 0;
	}else{
		return 1;
	}
	
}

void all_comment(){
	validate_Enter = 1;
}


int main(){

	if (! read_file()){
		return 0;
	}
	system_goal();
	//recope();
	return 0;
}