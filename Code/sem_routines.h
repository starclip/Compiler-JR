// Rutinas semánticas.

char *messageData = "section .data  	\n \
											\n \
	; En esta sección se declaran las variables. \n \
	; Se reserva la memoria necesaria.			 \n \
	; Se declaran las constantes.				 \n \n \
	;";

char *messageText = "section .text    \n \
											\n \
	; En esta sección se realizan las operaciones. 	\n \
	; Se escribe el código necesario para ejecutar.	\n \
	; Realiza los llamados adecuados.				\n \n \
global    _start   \n \
											\n \
_start:  	\n \n ";

char *messageFinish = " \n \
	mov eax, 1 \n \
	int 0x80 \n ";


char *messageRead = "  \n \
	; Leer de consola -> read \n \
	; \n \
	push eax 	\n \
	push ebx 	\n \
	push ecx 	\n \
	push edx 	\n \
			 	\n \
	mov eax, 3 	; EAX -> read  \n \
	mov ebx, 0 	; EBX -> input \n \
	mov ecx, %s ; ECX -> valor en que se almacena   \n \
	mov edx, %s	; EDX -> largo de lo que voy a leer \n \
	int 0x80 	; llamada al sistema operativo      \n \
			 	\n \
	pop edx 	\n \
	pop ecx 	\n \
	pop ebx 	\n \
	pop eax 	\n \
	; \n \
		";

char *messageWrite = "  \n \
	; Escribir en consola -> write \n \
	; \n \
	push eax 	\n \
	push ebx 	\n \
	push ecx 	\n \
	push edx 	\n \
			 	\n \
	mov eax, 4 	; EAX -> write  \n \
	mov ebx, 1 	; EBX -> input  \n \
	mov ecx, %s ; ECX -> valor que se va a imprimir     \n \
	mov edx, %s	; EDX -> largo de lo que voy a escribir \n \
	int 0x80 	; llamada al sistema operativo      \n \
			 	\n \
	pop edx 	\n \
	pop ecx 	\n \
	pop ebx 	\n \
	pop eax 	\n \
	; \n \
		";

char *messageTwoOp = "\t; %s  op1, op2, result \n \
	; ---------------------------  \n \
	push eax \n \
	push ebx \n \
	mov eax, [esp + %s] \n \
	mov ebx, [esp + %s] \n \
	%s eax, ebx \n \
	mov [esp + %s], ax \n \
	pop ebx \n \
	pop eax \n \
	; ---------------------------  \n \n ";


char *messageTwoLi = "\t; %s Literal, result \n \
	mov eax, %s        \n \
	mov [esp + %s], ax \n \n";

char *messageOneOne = "\t; %s  op1, Literal, result \n \
	; ---------------------------  \n \
	mov ecx, eax \n \
	mov edx, ebx \n \
	mov eax, %s \n \
	mov ebx, [esp + %s] \n \
	%s eax, ebx \n \
	mov [esp + %s], ax \n \
	mov eax, ecx \n \
	mov ebx, edx \n \
	; ---------------------------  \n \n ";

char *messageMovID = "\t; mov destino, origen \n \
	; ---------------------------  \n \
	mov ecx, eax  \n \
	mov eax, [esp + %s] \n \
	mov [esp + %s], ax \n \
	mov eax, ecx  \n \
	; ---------------------------  \n \
	";

char *messageMovL = "\t; movLiteral destino, origen L \n \
	; ---------------------------  \n \
	mov eax, %s         \n \
	mov [esp + %s], ax  \n \
	; ---------------------------  \n \
	";

char *messageDec = "\t; declare variable \n \
	; ---------------------------  \n \
	mov eax, 0				 \n \
	mov [esp + %s], ax   \n \n \
	";


// ¿Está en la tabla de simbolos?
extern int lookup(string s);

// Pongalo en la tabla de simbolos.
extern void enter(string s);

// Función que genera el código en ensamblador
void generate(string s, string m, string l, string p);

// Función que extrae 
char *extract(expr_rec source);

// Función que revisa el id de una variable
void check_id(string s);

// Obtener el temporal.
char *get_temp(void);

// Iniciar la llamada.
void start(void);

// Finalizar.
void finish(void);

// Asignar el valor a unas variables.
void assign(expr_rec target, expr_rec source);

// Obtiene el operador que este ejecutando en este momento.
op_rec process_op(void);

// Realiza 
expr_rec gen_infix(expr_rec e1, op_rec op, expr_rec e2);

// Leer variable.
void read_id(expr_rec in_var);

// Revisa si la palabra existe llamando a check id
expr_rec process_id(void);

// Convierte un literal a número.
expr_rec process_literal(void);

// 
void write_expr(expr_rec out_expr);
//

expr_rec constant_folding(expr_rec e1, op_rec op, expr_rec e2);

expr_rec if_condition(expr_rec left_operand, expr_rec center_operand, expr_rec right_operand);