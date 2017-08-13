/*
	scanner.c
	Toma el fuente y obtiene los tokens
*/
#include "scanner.h"

// Resetea el buffer donde se almacena un token
void clear_buffer(void){
	
	pos_token_buffer = 0;
	for (int i=0; i < LEN_TOKEN_BUFFER; i++){
		token_buffer[i] = 0;
	}
}

//Guarda el texto del token para los identificadores
void buffer_char(int in_char){

	token_buffer[pos_token_buffer] = in_char;
	pos_token_buffer++;
}

//Revisa el token_buffer y retorna el token correspondiente
//Determina si ese token es una palabra reservada
token check_reserved(void){

	if (strcmp(token_buffer, reserved_words[0]) == 0){
		return BEGIN;
	}
	if (strcmp(token_buffer, reserved_words[1]) == 0){
		return END;
	}
	if (strcmp(token_buffer, reserved_words[2]) == 0){
		return READ;
	}
	if (strcmp(token_buffer, reserved_words[3]) == 0){
		return WRITE;
	}
	return ID;
}

void lexical_error(int in_char){
	printf("%s %c %s %s\n", "Lexical Error. No se reconoce el caracter: ", in_char, " esta en: ", token_buffer);
}

//Lee caracteres y les determina su token
//Modificar para usar tablad de simbolos
token scanner(void){

	int in_char, c;
	clear_buffer();

	while ((in_char = getc(file)) != EOF){	//Obtener caracteres hasta el fin de archivo

		if (isspace(in_char)){
			continue;	// Ignora los espacios

		}else if (isalpha(in_char)){ // es un caracter alfabetico

			buffer_char(in_char);

			for (c = getc(file); isalnum(c) || c == '_'; c = getc(file)){
				buffer_char(c);
			}

			ungetc(c, file);
			return check_reserved();

		}else if (isdigit(in_char)){ // numero

			buffer_char(in_char);

			for (c = getc(file); isdigit(c); c = getc(file)){
				buffer_char(c);
			}

			ungetc(c, file);
			return INTLITERAL;

		}else if (in_char == '('){
			return LPAREN;

		}else if (in_char == ')'){
			return RPAREN;

		}else if (in_char == ';'){
			return SEMICOLON;

		}else if (in_char == ','){
			return COMMA;

		}else if (in_char == '+'){
			return PLUSOP;

		}else if (in_char == ':'){
			c = getc(file);
			if (c == '='){
				return ASSIGNOP;
			}else{
				ungetc(c, file);
				lexical_error(in_char);
			}

		}else if (in_char == '-'){
			c = getc(file);
			if (c == '-'){
				do 
					in_char = getc(file);
				while (in_char != '\n');
			}else{
				ungetc(c, file);
				return MINUSOP;
			}
		}else{
			lexical_error(in_char);
		}	

	}

	return SCANEOF;	
}
