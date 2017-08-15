/*
	sem_routines.h

	COMPILADOR MICRO -> x86

	Created by: 
		Jason Barrantes Arce.
		Randy Morales Gamboa

*/

char *messageData = "section     .data	  	\n \
											\n \
	;-------------------------------------------- \n \
	; En esta sección se declaran las variables. \n \
	; Se reserva la memoria necesaria.			 \n \
	; Se declaran las constantes.				 \n \
	;------------------------------------------- \n \
												 \n \
	global buffer_input   ; Buffer donde se almacenarán los valores.			\n \
	global len_input	  ; largo del buffer.									\n \
	global buffer_output  ; Buffer donde se escribirán los valores a imprimir.	\n \
	global len_output	  ; largo del buffer.									\n \
																				\n \
	buffer_input  db '*******************************', 0xa						\n \
	len_input      equ $ - buffer_input											\n \
	buffer_output db '*******************************', 0xa                     \n \
	len_output      equ $ - buffer_input 										\n \
	\n ";

char *messageText = "section     .text	  	\n \
											\n \
	;----------------------------------------------- \n \
	; En esta sección se realizan las operaciones. 	\n \
	; Se escribe el código necesario para ejecutar.	\n \
	; Realiza los llamados adecuados.				\n \
	;---------------------------------------------- \n \
												    \n \
extern buffer_input  	  	  \n \
extern len_input			  \n \
extern buffer_output		  \n \
extern len_output			  \n \
global      _start  		  \n \
							  \n \
; Funciones con etiquetas para desplazarme por el archivo						\n \
; //////////////////////////////////////////////////////////////////////////////////////////////////////////////// \n \n \
; Funciones \n \
												\n \
; # Atoi = edi -> contador; esi -> largo; eax -> número_final; \n \n \
atoi:						\n \
	xor ecx, ecx   						; Limpio el buffer \n \
	mov cl, [buffer_input + esi]  	 						 \n \
	sub cl, '0'							; Convierto a decimal  \n \
	mul ebx 								; Multiplico por diez  \n \
	add eax, ecx 						; Agrego el caracter convertido \n \
	inc esi 							\n \
	cmp edi, esi 						; comparo si ya llegue al final \n \
	jne atoi							; siga  \n \
	ret     							 \n \n \n \
itoa:  						\n 	\
	; Me convierte a ASCII \n \n	\
	xor edx, edx						; Limpio el registro edx  \n 	\
	div ebx								; eax:dividendo y resultado, edx:residuo, ebx:divisor \n 	\
	add dl, '0'							; Aplique la conversion a ASCII   \n 	\
	mov [buffer_output + esi], dl 		; Mueva a resultado en la posicion (UM, C, D, U) \n 	\
	dec esi								; Decremento la actual posición. \n 	\
	cmp esi, 0							; Si ya llegue al inicio, ponga cero. \n 	\
	jne itoa  							; Si no realice de nuevo	\n 	\
										; siga  \n \
	ret 								\n \n \n \
_start:  	\n \n ; Code \n \
	xor eax, eax \n ";

char *messageFinish = "\t Salir del programa  \n  \
	; ---------------------------  \n  \
	mov eax, 1 \n \
	int 0x80 \n ";


char *messageRead = "\t; Leer de consola -> read \n \
	; ------------------------  \n \
	; \n \n \
	mov eax, 3 	; EAX -> read  \n \
	mov ebx, 0 	; EBX -> input \n \
	mov ecx, buffer_input ; ECX -> valor en que se almacena   \n \
	mov edx, len_input	; EDX -> largo de lo que voy a leer \n \
	int 0x80 	; llamada al sistema operativo    \n  \n \
	mov edi, eax \n \
	xor edx, edx \n \
 	xor esi, esi \n \
 	xor eax, eax \n \
 	mov ebx, 10  \n \
 				 \n \
 	dec edi		 \n \
 	call atoi   \n \
 	mov [esp + %s], ax \n \
	; Realiza el read -> y se almacena en memoria. \n \
	; ---------------------------  \n \n ";

char *messageWrite = "\t; Escribir en consola -> write \n \
	; ---------------------------  \n \
	; \n \
	mov ax, [esp + %s]  ; pongo en el ax e\n \
	mov ebx, 10 \n \
	mov esi, 30 \n \
	call itoa   \n \
	\n \
	mov eax, 4 	; EAX -> write  \n \
	mov ebx, 1 	; EBX -> input  \n \
	mov ecx, buffer_output ; ECX -> valor que se va a imprimir     \n \
	mov edx, len_output	; EDX -> largo de lo que voy a escribir \n \
	int 0x80 	; llamada al sistema operativo      \n \
	; ---------------------------  \n \n ";

char *messageTwoOp = "\t; %s  op1, op2, result \n \
	; ---------------------------  \n \
	mov eax, [esp + %s] \n \
	mov ebx, [esp + %s] \n \
	%s eax, ebx \n \
	mov [esp + %s], ax \n \
	; ---------------------------  \n \n ";


char *messageTwoLi = "\t; %s Literal, result \n \
 	; ---------------------------  \n \
	mov eax, %s        \n \
	mov [esp + %s], ax   \n \
	; ---------------------------  \n \n ";

char *messageOneOne = "\t; %s  op1, Literal, result \n \
	; ---------------------------  \n \
	mov eax, %s \n \
	mov ebx, [esp + %s] \n \
	%s eax, ebx \n \
	mov [esp + %s], ax \n \
	; ---------------------------  \n \n ";

char *messageMovID = "\t; mov destino, origen \n \
	; ---------------------------  \n \
	mov eax, [esp + %s] \n \
	mov [esp + %s], ax \n \
	; ---------------------------  \n \
	";

char *messageMovL = "\t; movLiteral destino, origen L \n \
	; ---------------------------  \n \
	mov eax, %s         \n \
	mov [esp + %s], ax  \n \
	; ---------------------------  \n \
	";

char *messageDec = " \n \tpush eax \n";


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

void write_expr(expr_rec out_expr);

expr_rec constant_folding(expr_rec e1, op_rec op, expr_rec e2);

expr_rec if_condition(expr_rec left_operand, expr_rec center_operand, expr_rec right_operand);