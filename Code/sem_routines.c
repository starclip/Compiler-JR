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

	int tempVal;
	// Agregué el primer elemento.
	if (table.head == NULL){
		table.head = table.current = (struct elem *) malloc (sizeof(struct elem));
		strcpy(table.current->values.name, word);
		table.current->values.val = 0;
		return;
	}
	// Agregue el siguiente elemento.
	tempVal = table.current->values.val;
	table.current->next = (struct elem *) malloc (sizeof(struct elem));
	table.current = table.current->next;
	strcpy(table.current->values.name, word);
	table.current->values.val = tempVal + 4;
}

void recope(){
	table.current = table.head;
	while(table.current != NULL){
		// printf(" Name - %s - Memory Direction - %d  \n", table.current->values.name, table.current->values.val);
		table.current = table.current->next;
	}

}

void generate(string op_code, string op1, string op2, string result_field){
	// llama a extract()
	char c1, c2, c3, c4;
	
	fprintf(current_file, "\t%s %s %s %s\n", op_code, op1, op2, result_field);
}

char *extract_op(op_rec source){
	static char assembly_op[3];
	if (source.operator == MINUS){
		strcpy(assembly_op, "sub");
	}else{
		strcpy(assembly_op, "add");
	}
	return assembly_op;
}

// Todavía presenta errores.
char *extract_exp(expr_rec source){

	char *assembly_var = malloc(MAXIDLEN);
	if (source.kind == IDEXPR || source.kind == TEMPEXPR){
		strcpy(assembly_var, source.name);
	}else{
		sprintf(assembly_var, "%d", source.val);
	}
	return assembly_var;

}

void check_id(string s){

	if (! lookup(s)){
		char *message = malloc(10);
		enter(s);
		sprintf(message, "\t%s db %d", s, table.current->values.val);

		current_file = file_sData;
		fprintf(current_file, "%s \n", message);
		free(message);
	}
}

//almacena temporales
char *get_temp(void){
	
	static int max_temp = 0;		/* max temporary allocated so far */
	static char tempname[MAXIDLEN];
	max_temp++;
	sprintf(tempname, "Temp%d", max_temp);
	check_id(tempname);
	return tempname;
}

//Inicio de compilación
void start(void){
	// generate section.data
	file_sData = fopen("sectionData.txt", "w");
	file_sText = fopen("sectionText.txt", "w");

	fprintf(file_sData, "%s \n", messageData);
	fprintf(file_sText, "%s \n", messageText);	

}

// Finalizar.
void finish(void){
	/* Generate code to finish program. */
	current_file = file_sData;
	
	fprintf(file_sText, "%s \n", messageFinish);
	fclose(file_sData);
	fclose(file_sText);
}

// Asignar el valor a las variables (mov var 80).
void assign(expr_rec target, expr_rec source){

	char *message;
	current_file = file_sText;
	if (source.kind == LITERALEXPR){
		message = messageMovL;
	}else{
		message = messageMovID;
	}
	current_file = file_sText;
	fprintf(current_file, message, target.name, source.name);
	//generate("mov", target.name, extract_exp(source), "");
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

expr_rec constant_folding(expr_rec e1, op_rec op, expr_rec e2){
	expr_rec e_rec;
	char arr[5];
	e_rec.kind = LITERALEXPR;
	if (op.operator == MINUS){
		e_rec.val = e1.val - e2.val;
		strcpy(arr, "sub");
	}else{
		e_rec.val = e1.val + e2.val;
		strcpy(arr, "add");
	}
	strcpy(e_rec.name, get_temp());
 
	char *message = messageTwoLi;
	current_file = file_sText;
	fprintf(current_file, message, arr, e_rec.name, extract_exp(e_rec));
	e_rec.kind = TEMPEXPR;
	return e_rec;


}

expr_rec gen_infix(expr_rec e1, op_rec op, expr_rec e2){
	expr_rec e_rec;
	char *message;
	e_rec.kind = TEMPEXPR;
	string op_e1;
	string op_e2;
	string op_op;

	current_file = file_sText;

	if (e1.kind == LITERALEXPR && e2.kind == LITERALEXPR){
		return constant_folding(e1, op, e2);
	}

	if (e1.kind == LITERALEXPR || e2.kind == LITERALEXPR){
		message = messageOneOne;
		if (e2.kind == LITERALEXPR){
			strcpy(op_e1, extract_exp(e2));
			strcpy(op_e2, extract_exp(e1));
		}else{
			strcpy(op_e1, extract_exp(e1));
			strcpy(op_e2, extract_exp(e2));
		}
	}else{
		message = messageTwoOp;
		strcpy(op_e1, extract_exp(e1));
		strcpy(op_e2, extract_exp(e2));
	}

	strcpy(op_op, extract_op(op));
	strcpy(e_rec.name, get_temp());

	current_file = file_sText;
	fprintf(current_file, message, op_op, op_e1, op_e2, op_op, e_rec.name);
	return e_rec;
}

void read_id(expr_rec in_var){
	/* Generate code for read */
	current_file = file_sText;
	//generate("Read", in_var.name, "Integer", "");
	char *message = messageRead;
	fprintf(current_file, message, in_var.name, "lenght");
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
	current_file = file_sText;
	//generate("Write", extract_exp(out_expr), "Integer", "");
	char *message = messageWrite;
	fprintf(current_file, message, out_expr.name, "lenght");
}
