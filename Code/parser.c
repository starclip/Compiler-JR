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


/*

token next_token(void){
	
	if (temp_token_ptr == NULL){
		temp_token = scanner();
		temp_token_ptr = &temp_token;;
		//temp_token_ptr = &temp;
		//return temp;
	}
	//printf(" * %d\n", *temp_token_ptr);
	return *temp_token_ptr;
}


void match(token tok){

	if (temp_token_ptr == NULL){
		temp_token = scanner();
		//printf("Parameters -> %d, Scanner -> %d\n", tok, temp_token);
		if(temp_token == tok){
			//printf("%s\n","True");
			current_token = tok;
		}else{
			syntax_error(tok);
		}
		return;
	}

	if (*temp_token_ptr == tok){
		//printf("Parameters -> %d, Scanner -> %d\n", tok, *temp_token_ptr);
		current_token = *temp_token_ptr;
		temp_token_ptr = NULL;
	}
}

*/

// Match y next_token no funcionan con write o con otros tipos de errores.

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
		syntax_error(tempTok);
	}
	
	
}


void syntax_error(token tok){	//FALTA HACER

	if (tok == BEGIN){
		printf("%s\n", "Error de sintaxis. El BEGIN no va en esa parte.");
	}else if(tok == END){
		printf("%s\n", "Error de sintaxis. El END no va en esa parte");
	}else if(tok == READ){
		printf("%s\n", "Error de sintaxis. El READ no va en esa parte");
	}else if(tok == WRITE){
		printf("%s\n", "Error de sintaxis. El WRITE no va en esa parte");
	}else if(tok == ID){
		printf("%s\n", "Error de sintaxis. El ID no va en esa parte");
	}else if(tok == INTLITERAL){
		printf("%s\n", "Error de sintaxis. El INTLITERAL no va en esa parte");
	}else if(tok == LPAREN){
		printf("%s\n", "Error de sintaxis. El LPAREN no va en esa parte");
	}else if(tok == RPAREN){
		printf("%s\n", "Error de sintaxis. El RPAREN no va en esa parte");
	}else if(tok == SEMICOLON){
		printf("%s\n", "Error de sintaxis. El SEMICOLON no va en esa parte");
	}else if(tok == COMMA){
		printf("%s\n", "Error de sintaxis. El COMMA no va en esa parte");
	}else if(tok == ASSIGNOP){
		printf("%s\n", "Error de sintaxis. El ASSIGNOP no va en esa parte");
	}else if(tok == PLUSOP){
		printf("%s\n", "Error de sintaxis. El PLUSOP no va en esa parte");
	}else if(tok == MINUSOP){
		printf("%s\n", "Error de sintaxis. El MINUSOP no va en esa parte");
	}else if(tok == SCANEOF){
		printf("%s\n", "Error de sintaxis. El SCANEOF no va en esa parte");
	}
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


	expr_rec left_operand, right_operand;
	op_rec op;

	primary(&left_operand);	//
	token t = next_token();
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
	recope();
	return 0;
}