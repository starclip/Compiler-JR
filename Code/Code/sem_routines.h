// Rutinas semánticas.

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


