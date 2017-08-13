// Rutinas semanticas del programa
// Se encargan de traducir los tokens a x86

#include "sem_routines.h"

//revisa si el parametro se encuentra en la tabla de símbolos
int lookup(string word){

	// Si está vacía, no haga nada.
	if (table.head == NULL){
		return 0;
	}
	// Recorra la tabla de símbolos.
	table.current = table.head;

	while(table.current != NULL){
		if (strcmp(word, table.current->values.name) == 0){
			return 1;
		}
		table.temp = table.current;
		table.current = table.current->next;
	}
	table.current = table.temp;
	return 0;
}

//ingresa el parametro en la tabla de simbolos
void enter(string word){

	// Agregué el primer elemento.
	if (table.head == NULL){
		table.head = table.current = (struct elem *) malloc (sizeof(struct elem));
		strcpy(table.current->values.name, word);
		return;
	}
	table.current->next = (struct elem *) malloc (sizeof(struct elem));
	table.current = table.current->next;
	strcpy(table.current->values.name, word);
}

void recope(){
	table.current = table.head;
	while(table.current != NULL){
		printf("%s\n", table.current->values.name);
		table.current = table.current->next;
	}

}

void generate(string op_code, string op1, string op2, string result_field){
	// llama a extract()
	char c1, c2, c3, c4;
	
	fprintf(file_sText, "%s %s %s %s\n", op_code, op1, op2, result_field);
}

char *extract_op(op_rec source){
	static char assembly_op[3];
	if (source.operator == MINUS){
		strcpy(assembly_op, "SUB");
	}else{
		strcpy(assembly_op, "ADD");
	}
	return assembly_op;
}

// Todavía presenta errores.
char *extract_exp(expr_rec source){

	static char assembly_var[MAXIDLEN];
	if (source.kind == IDEXPR || source.kind == TEMPEXPR){
		strcpy(assembly_var, source.name);
		return assembly_var;
	}else{
		sprintf(assembly_var, "%d", source.val);
		return assembly_var;
	}

}

void check_id(string s){

	if (! lookup(s)){
		enter(s);
		generate("Declare", s, "Integer", "");
	}
}

//almacena temporales
char *get_temp(void){
	
	static int max_temp = 0;		/* max temporary allocated so far */
	static char tempname[MAXIDLEN];
	max_temp++;
	sprintf(tempname, "Temp&%d", max_temp);
	check_id(tempname);
	return tempname;
}

//Inicio de compilación
void start(void){
	// generate section.data
	file_sData = fopen("sectionData.txt", "w");
	file_sText = fopen("sectionText.txt", "w");
}

// Finalizar.
void finish(void){
	/* Generate code to finish program. */
	generate("Halt", "", "", "");
	fclose(file_sData);
	fclose(file_sText);
}

// Asignar el valor a las variables (mov var 80).
void assign(expr_rec target, expr_rec source){

	generate("mov", extract_exp(source), target.name, "");
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
	expr_rec e_rec;
	/* An expr_rec with temp variant set. */
	e_rec.kind = TEMPEXPR;

	/*
	 * Generate code for infix operation.
	 * Get result temp and set up semantic record
	 * for result.
	*/
	strcpy(e_rec.name, get_temp());
	generate(extract_op(op), extract_exp(e1), extract_exp(e2), e_rec.name);
	return e_rec;
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
	generate("Write", extract_exp(out_expr), "Integer", "");
}
