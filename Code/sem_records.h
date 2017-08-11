#define MAXWORDS 512
#define MAXIDLEN 33
typedef char string[MAXIDLEN];

char table_symbol[MAXWORDS][MAXIDLEN];

typedef struct operator
{
	enum op {PLUS, MINUS} operator;
} op_rec;

// Tipos de expresion

enum expr { IDEXPR, LITERALEXPR, TEMPEXPR };

/*  */
typedef struct expression {
	enum expr kind;
	union {
		string name; 	/* for IDEXPR, TEMPEXPR */
		int val;		/* for LITERALEXPR */
	};
} expr_rec;