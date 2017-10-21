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
  ALC,
  FRE,
  ATR,
  SIS, //chamada ao sistema (mov, extrair, depositar e atk)
} OpCode;

/* Tipo de terreno */
typedef enum {
  ESTRADA,
  TERRA,
  MONTANHA,
  AGUA,
  LAMA
} Terreno;

/* Célula */
typedef struct {
  Terreno terreno;
  short int x;
  short int y;
  short int cristais;
  short int ocupado;
  short int base;
  Maquina* maquina_na_celula;
} Celula;

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
  OPERANDO op;
} INSTR;

/* Instrução
typedef struct {
  OpCode instr;
  int argumento;
  OPERANDO op;
} INSTR;*/
