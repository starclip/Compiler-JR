//Asociar cada semantic record con cada tipo de simbolo gramatical
//Cada tipo de simbolo tendra el mismo dato en su semantic record

#define MAXWORDS 512
#define MAXIDLEN 33
typedef char string[MAXIDLEN];




//PROPONGO QUITAR ESTO Y USAR SOLO EL expr_rec
//Ese op_rec no hace mucho, se puede cambiar
typedef struct operator{

	enum op {PLUS, MINUS} operator;

} op_rec;

// Tipos de expresion
enum expr { IDEXPR, LITERALEXPR, TEMPEXPR };

typedef struct expression {

	enum expr kind;
	string name;
	int val;
	
} expr_rec;

/*  
	Elem -> es equivalente a una fila en la tabla de símbolos.
	| Next | Nombre | Tipo | Valor |
	--------------------------------
	Next -> almacena el puntero a la siguiente fila.
	Values -> almacena el nombre, tipo y valor.
*/

struct elem {
	struct elem *next;
	expr_rec values;
};


/*
	Tbl_symbol -> es la tabla de símbolos.
	-------------------------------------
	Head -> Es un puntero a la primera fila de la tabla de símbolos.
	Current -> Es un puntero a la fila en donde me encuentro.
	Temp -> Es un puntero temporal para realizar intercambios.
*/

typedef struct table_symbol{
	struct elem* head;
	struct elem* current;
	struct elem* temp;
} tbl_symbol;

/*
// 	La tabla de símbolos es una estructura que simila una lista enlazada, 
	donde almacena una estructura elem que a su vez simila ser una lista de valores necesarios.
*/
tbl_symbol table;

