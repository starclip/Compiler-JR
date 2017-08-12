//Asociar cada semantic record con cada tipo de simbolo gramatical
//Cada tipo de simbolo tendra el mismo dato en su semantic record

#define MAXWORDS 512
#define MAXIDLEN 33
typedef char string[MAXIDLEN];

char table_symbol[MAXWORDS][MAXIDLEN];


//PROPONGO QUITAR ESTO Y USAR SOLO EL expr_rec
//Ese op_rec no hace mucho, se puede cambiar
typedef struct operator{

	enum op {PLUS, MINUS} operator;

} op_rec;

// Tipos de expresion
enum expr { IDEXPR, LITERALEXPR, TEMPEXPR };

typedef struct expression {

	enum expr kind;
	union {
		string name; 	/* for IDEXPR, TEMPEXPR */
		int val;		/* for LITERALEXPR */
	};
	
} expr_rec;