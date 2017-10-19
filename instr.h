/* Códigos das instruções */
typedef enum {
  PUSH,
  POP,
  DUP,
  ADD,
  SUB,
  MUL,
  DIV,
  JMP,
  JIT,
  JIF,
  CALL,
  RET,
  STS,
  RCS,
  EQ,
  GT,
  GE,
  LT,
  LE,
  NE,
  STO,
  RCL,
  END,
  PRN,
  STL,
  RCE,
  FRE,
  ALC,
  ATR,
  SIS, //chamada ao sistema (mov, extrair, depositar e atk)
} OpCode;

/* Tipos dos operandos */
/* no momento, são todos inteiros */
typedef enum {
  NUM,
  ACAO,
  VAR,
  CELULA,
  MOV,
  EXTR,
  POR,
  ATK,
} Tipo;

/*typedef struct{
  int x;
  int y;
  int cristais;
} Atributo;*/

/* Operando */
/* typedef struct { */
/*   Tipo t; */
/*   union { */
/* 	int n; */
/* 	int ac; */
/* 	int v; */
/*   }; */
/* } OPERANDO; */
//typedef int OPERANDO;

typedef struct {
   Tipo t;
   int valor;
   /*union {
    int n;
    int ac;
    int v;
   };*/
} OPERANDO;


/* Instrução */
typedef struct {
  OpCode instr;
  int argumento;
  OPERANDO op;
} INSTR;
