// Rutinas semanticas del programa
#include "sem_routines.h"

int lookup(string s){
	// Falta implementar

}

void enter(string s){
	// Falta implementar
}

void generate(string s, string m, string l, string p){
	// Falta implementar
}

char *extract(expr_rec source){
	// Falta implementar
}

void check_id(string s){

	if (! lookup(s)){
		enter(s);
		generate("Declare", s, "Integer", "");
	}
}

char *get_temp(void){
	/* max temporary allocated so far */
	static int max_temp = 0;
	static char tempname[MAXIDLEN];
	max_temp++;
	sprintf(tempname, "Temp&%d", max_temp);
	check_id(tempname);
	return tempname;
}

// Iniciar la llamada.
void start(void){
	/* Semantic initializations, none needed */
}

// Finalizar.
void finish(void){
	/* Generate code to finish program. */
	generate("Halt", "", "", "");
}

// Asignar el valor a unas variables.
void assign(expr_rec target, expr_rec source){
	/* Generate code for assignment */
	generate("Store", extract(source), target.name, "");
}

// Realiza el proceso de la operación.
op_rec process_op(void){
	/* Produce operator descriptor. */
	op_rec o;

	if (current_token == PLUSOP){
		o.operator = PLUS;
	}else{
		o.operator = MINUS;
	}
	return o;
}

expr_rec gen_infix(expr_rec e1, op_rec op, expr_rec e2){
	expr_rec erec;
	/* An expr_rec with temp variant set. */
	erec.kind = TEMPEXPR;

	/*
	 * Generate code for infix operation.
	 * Get result temp and set up semantic record
	 * for result.
	*/
	strcpy(erec.name, get_temp());
	generate(extract(op), extract(e1), extract(e2), erec.name);
	return erec;
}

void read_id(expr_rec in_var){
	/* Generate code for read */
	generate("Read", in_var.name, "Integer", "");
}

expr_rec process_id(void){
	expr_rec t;
	/*
	 * Declare ID and build a corresponding semantic record.
	*/
	check_id(token_buffer);
	t.kind = IDEXPR;
	strcpy(t.name, token_buffer);
	return t;
}

expr_rec process_literal(void){
	expr_rec t;
	/*
	 * Convert literal to a numeric representation and
	 * build semantic record.
	 */
	t.kind = LITERALEXPR;
	(void) sscanf(token_buffer, "%d", & t.val);
	return t;
}

void write_expr(expr_rec out_expr){
	generate("Write", extract(out_expr), "Integer", "");
}
