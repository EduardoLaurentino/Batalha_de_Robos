/* Compilador */

%{
#include <stdio.h>
#include <math.h>
#include "symrec.h"
#include "acertos.h"
#include "instr.h"
  
int yylex();
void yyerror(char const *);
int compila(FILE *, INSTR *);
static int ip  = 0;					/* ponteiro de instruções */
static int mem = 6;					/* ponteiro da memória */
static INSTR *prog;
static int parmcnt = 0;		/* contador de parâmetros */

void AddInstr(OpCode op, OPERANDO valor) {
  prog[ip++] = (INSTR) {op,  {valor.t, {valor.val}}};
}
%}

/*  Declaracoes */
%union {
  double val;
  /* symrec *cod; */
  char cod[30];
}

/* %type  Expr */

%token <val> NUMt
%token <cod> ID
%token ADDt SUBt MULt DIVt ASGN OPEN CLOSE RETt EOL
%token EQt NEt LTt LEt GTt GEt ABRE FECHA SEP
%token IF /*ELSE*/ WHILE FUNC PRINT

%token MOVt POEt EXTt ATKt

%right ASGN
%left ADDt SUBt
%left MULt DIVt
%left NEG
%right PWR
%left LTt GTt LEt GEt EQt NEt


/* Gramatica */
%%

Programa: Comando
       | Programa Comando
	   ;

Comando: Expr EOL
       | Cond
       //???????| Cond Else
       | Loop
       | Func
	   | PRINT Expr EOL { AddInstr(PRN, {NUM, 0});}
       | RETt EOL {
		 	     AddInstr(LEAVE, {NUM, 0});
			     AddInstr(RET, {NUM, 0});
 			  }
       | RETt OPEN Expr CLOSE EOL {
		 	     AddInstr(LEAVE, {NUM, 0});
			     AddInstr(RET, {NUM, 0});
 		      }
 		      
 	   // NOVOS COMANDOS ~~ESPECÍFICOS~~
       | MOVt OPEN NUMt CLOSE EOL { AddInstr(SIS, {MOV, $3}); }
       | POEt OPEN NUMt CLOSE EOL { AddInstr(SIS, {POR, $3}); }
       | EXTt OPEN NUMt CLOSE EOL { AddInstr(SIS, {EXTR, $3}); }
       | ATKt OPEN NUMt CLOSE OPEN NUMt CLOSE EOL { AddInstr(SIS, {ATK, $3, $6}); }
       
       // FALTA ATR
 	   // FALTA TRATAMENTO DE ERROS?
 	   // VARIÁVEIS ESPECIAIS SÃO NA PARTE COMENTADA MAIS ABAIXO? (ID PONTO ETC)
       
	   /* | EOL {printf("--> %d\n", ip);} */
;

Expr: NUMt {  AddInstr(PUSH, {NUM, $1});}
    | ID   {
	         symrec *s = getsym($1);
			 if (s==0) s = putsym($1); /* não definida */
			 AddInstr(RCL, {NUM, s->val});
	       }
	| ID ASGN Expr {
	         symrec *s = getsym($1);
			 if (s==0) s = putsym($1); /* não definida */
			 AddInstr(STO, {NUM, s->val});
 		 }
	/* | ID PONTO NUMt  {  % v.4 */
	/*          symrec *s = getsym($1); */
	/* 		 if (s==0) s = putsym($1); /\* não definida *\/ */
	/* 		 AddInstr(PUSH, s->val); */
	/* 		 AddInstr(ATR, $3); */
 	/* 	 } */
	| Chamada 
    | Expr ADDt Expr { AddInstr(ADD,  {NUM, 0});}
	| Expr SUBt Expr { AddInstr(SUB,  {NUM, 0});}
	| Expr MULt Expr { AddInstr(MUL,  {NUM, 0});}
	| Expr DIVt Expr { AddInstr(DIV,  {NUM, 0});}
    | '-' Expr %prec NEG  { printf("  {CHS,  {NUM, 0}},\n"); }
	| OPEN Expr CLOSE
	| Expr LTt Expr  { AddInstr(LT,   {NUM, 0});}
	| Expr GTt Expr  { AddInstr(GT,   {NUM, 0});}
	| Expr LEt Expr  { AddInstr(LE,   {NUM, 0});}
	| Expr GEt Expr  { AddInstr(GE,   {NUM, 0});}
	| Expr EQt Expr  { AddInstr(EQ,   {NUM, 0});}
	| Expr NEt Expr  { AddInstr(NE,   {NUM, 0});}
;

Cond: IF OPEN Expr {
  	  	 	   salva_end(ip);
			   AddInstr(JIF, {NUM, 0});
 		 }
		 CLOSE  Bloco {
		   prog[pega_end()].op.val.n = ip;
		 };

/*????Else: ELSE Bloco {
		 };
????????*/


Loop: WHILE OPEN  { salva_end(ip); }
	  		Expr  { salva_end(ip); AddInstr(JIF, {NUM, 0}); }
	  		CLOSE Bloco {
			  int ip2 = pega_end();
			  AddInstr(JMP, {NUM, pega_end()});
			  prog[ip2].op.val.n = ip;
			}; 

Bloco: ABRE Comandos FECHA ;

Comandos: Comando 
    | Comandos Comando
	;

Func: FUNC ID
	  {
		salva_end(ip);
		AddInstr(JMP, {NUM, 0});
		symrec *s = getsym($2);
		if (s==0) s = putsym($2);
		else {
		  yyerror("Função definida duas vezes\n");
		  YYABORT;
		}
		s->val = ip;
	  } OPEN
	  {
		newtab(0);
	  }
	  Args CLOSE  Bloco
	  {
		AddInstr(LEAVE, {NUM, 0});
		AddInstr(RET, {NUM, 0});
		prog[pega_end()].op.val.n = ip;
		deltab();
	  }
	  ;

Args: 
	| ID {
	  	 putsym($1);
	  }
    | Args SEP ID {
	  	 putsym($3);
	  }
	;

Chamada: ID OPEN
		 {
			 parmcnt = 0;
			 /* posição da memória mais avançada */
		 }
		 ListParms
		 {
		   symrec *s = getsym($1);
		   if (s == 0) {
			 yyerror("Função não definida\n");
			 YYABORT;
		   }
		   AddInstr(ENTRY, {NUM, lastval()});
		   /* Cópia dos parâmetros */
		   while (parmcnt > 0) 
			 AddInstr(STO, {NUM, --parmcnt});
		   AddInstr(CALL, {NUM, s->val});
		 }
	  	 CLOSE ;


ListParms:
	| Expr { parmcnt++;}
	| Expr { parmcnt++;} SEP ListParms
;

%%
extern FILE *yyin;

void yyerror(char const *msg) {
  fprintf(stderr,"ERRO: %s",msg);
}

int compilador(FILE *cod, INSTR *dest) {
  int r;
  yyin = cod;
  prog = dest;
  cleartab();
  ip = 0;
  r = yyparse();
  AddInstr(END, {NUM, 0});
  return r;
}
