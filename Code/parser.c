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
#include "sem_records.h"
#include "sem_routines.c"
//############################################### FUNCIONES PARSER ###############################################

token next_token(void){		// Es una función que se puede discutir para encontrar una mejor solución
	if (current_token_ptr == temp_token_ptr || temp_token_ptr == NULL){
		temp_token = scanner();
		temp_token_ptr = &temp_token;
	}
	return temp_token;
}

void match(token tok){ 		// Es una función que se puede discutir para encontrar una mejor solución

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
	current_token_ptr = temp_token_ptr;
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


void id_list(void){		// <id list> ::= ID {  , ID }
	
	match(ID);

	while(next_token() == COMMA){
		match(COMMA);
		match(ID);
	}
}

void add_op(void){		// <add op> ::= PLUSOP | MINUSOP
	
	token tok = next_token();

	if(tok == PLUSOP || tok == MINUSOP){
		match(tok);
	}else{
		syntax_error(tok);
	}
}

void primary(void){

	token tok = next_token();

	switch(tok){

		case LPAREN: 	// <primary> ::= ( <expression> )
			match(LPAREN);
			expression();
			match(RPAREN);
			break;

		case ID:		// <primary> ::= ID
			match(ID);
			break;

		case INTLITERAL:	// <primary> ::= INTLITERAL
			match(INTLITERAL);
			break;

		default:
			syntax_error(tok);
			break;
	}
}

void expression(void){		// <expression> ::= <primary> { <add op> <primary> }

	token t;
	primary();

	for(t = next_token(); t == PLUSOP || t == MINUSOP; t = next_token()){
		add_op();
		primary();
	}
}

void expr_list(void){		// <expr list> ::= <expression> {  , <expression> }

	expression();

	while(next_token() == COMMA){
		match(COMMA);
		expression();
	}
}


void statement(void){

	token tok = next_token();

	switch(tok){
		case ID:		//<statement> ::= ID := <expression> ;
			match(ID);
			match(ASSIGNOP);
			expression();
			match(SEMICOLON);
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

	match(BEGIN);
	statement_list();
	match(END);
}

void system_goal(void){		// <system goal> ::= <program> SCANEOF

	program();
	match(SCANEOF);
}

void read_file(){

	file = fopen("example_3.txt", "r");
	if (file == NULL){
		printf("%s\n", "El archivo no existe.");
	}else{
		return;
	}
	
}


int main(){

	printf("%s\n", "Compilando...\n");
	read_file();
	system_goal();
	return 0;
}