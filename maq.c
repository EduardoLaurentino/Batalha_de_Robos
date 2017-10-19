#include <stdio.h>
#include <stdlib.h>
#include "maq.h"
#include "arena.c"

/* #define DEBUG */

#ifdef DEBUG
#  define D(X) X
char *CODES[] = {
  "PUSH",
  "POP",
  "DUP",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "JMP",
  "JIT",
  "JIF",
  "CALL",
  "RET",
  "EQ",
  "GT",
  "GE",
  "LT",
  "LE",
  "NE",
  "STO",
  "RCL",
  "END",
  "PRN",
  "STL",
  "RCE",
  "CSIS" //chamada ao sistema (mov, extrair, depositar e atk)
};

/*
  "MOV",
  "EXTR",
  "POR"
  "ATK",*/

#else
#  define D(X)
#endif

static void Erro(char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void Fatal(char *msg, int cod) {
  Erro(msg);
  exit(cod);
}

Maquina *cria_maquina(INSTR *p) {
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->ip = 0;
  m->rbp = 0; //novo registrador
  m->prog = p;
  m->cristais = 0; //um robo começa com zero cristais????????
  m->x = /*aleatório?*/
  m->y = /*aleatório?*/
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define rbp (m->rbp) //novo registrador
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)
/*
// correlacionamos algumas ações de um robo com numeros.
#define mover 1
#define extrair 2
#define depositar 3
#define atacar 4

// fizemos uma correspondencia entre as vizinhancas de uma celula com numeros.
#define aqui 0
#define norte 1
#define nordeste 2
#define sudeste 3
#define sul 4
#define sudoeste 5
#define noroeste 6
*/

void exec_maquina(Maquina *m, int n) {
  int i;

  for (i = 0; i < n; i++) {
    OpCode   opc = prg[ip].instr;
    int arg = prg[ip].argumento;
    OPERANDO op = prg[ip].op;

    D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));

    switch (opc) {
      OPERANDO tmp;
    case PUSH:
      empilha(pil, arg);
      break;
    case POP:
      desempilha(pil);
      break;
    case DUP:
      tmp = desempilha(pil);
      empilha(pil, tmp);
      empilha(pil, tmp);
      break;
    case ADD:
      empilha(pil, desempilha(pil)+desempilha(pil));
      break;
    case SUB:
      tmp = desempilha(pil);
      empilha(pil, desempilha(pil)-tmp);
      break;
    case MUL:
      empilha(pil, desempilha(pil)*desempilha(pil));
      break;
    case DIV:
      tmp = desempilha(pil);
      empilha(pil, desempilha(pil)/tmp);
      break;
    case JMP:
      ip = arg;
      continue;
    case JIT:
      if (desempilha(pil) != 0) {
        ip = arg;
        continue;
      }
      break;
    case JIF:
      if (desempilha(pil) == 0) {
        ip = arg;
        continue;
      }
      break;

  //modificacoes para a utilizacao de variaveis locais com criacao de frames
  case CALL:
    empilha(exec, ip);
    empilha (exec, rbp);
    rbp = (exec->topo + 1); //correção do topo da pilha.
    ip = arg;
    continue;
  case RET:
    exec->topo = rbp;
    rbp = desempilha(exec);
    ip = desempilha(exec);
    break;

  case EQ:
    if (desempilha(pil) == desempilha(pil))
    empilha(pil, 1);
    else
    empilha(pil, 0);
    break;
  case GT:
    if (desempilha(pil) < desempilha(pil))
    empilha(pil, 1);
    else
    empilha(pil, 0);
    break;
  case GE:
    if (desempilha(pil) <= desempilha(pil))
    empilha(pil, 1);
    else
    empilha(pil, 0);
    break;
  case LT:
    if (desempilha(pil) > desempilha(pil))
    empilha(pil, 1);
    else
    empilha(pil, 0);
    break;
  case LE:
    if (desempilha(pil) >= desempilha(pil))
    empilha(pil, 1);
    else
    empilha(pil, 0);
    break;
  case NE:
    if (desempilha(pil) != desempilha(pil))
    empilha(pil, 1);
    else
    empilha(pil, 0);
    break;
  case STO:
    m->Mem[arg] = desempilha(pil);
    break;
  case RCL:
    empilha(pil,m->Mem[arg]);
    break;
  case END:
    return;
  case PRN:
    printf("%d\n", desempilha(pil));
    break;

	//novas funcoes
  case STL:
     exec->val[arg+rbp] = desempilha(pil);
    //m->Mem[arg+rbp] = desempilha(pil);
    break;
  case RCE:
    empilha(pil, exec->val[arg + rbp]);
    //empilha(pil, m->Mem[arg + rbp]);
    break;
  case ALC:
    empilhar(exec, exec->rbp)
    exec->rbp = exec->topo;
    exec->topo = exec->topo + arg;
    break;
  case FRE:
    exec->topo = exec-> topo - 5;
    break;
  case ATR:
    empilha(pil, desempilha(pil)[arg]); //verificar sintáxe
    break;
  case SIS:
    Sistema(op.t, m);
    break;
  }
  // Caso nao entre em nenhum "case", ele sai do switch e segue o baile?


  // novas funcoes - fase2
  // Saindo do switch de cima, sabemos que o opc = 'nulo' e, entao, fara um switch baseado no op (mover, extrair...)
/* No caso MOV, que eh um tipo da struct "OPERANDO", a variavel "int valor" representa a direção.

*/
 /* switch(op.t){
    case MOV:
      Sistema(op.t, op.valor, m);
      break;
    case EXTR:
      Sistema(extrair, m);
      break;
    case POR:
      Sistema(depositar, m);
      break;
    case ATK:
      Sistema(ataque, m);
      break;
  }  */


	D(imprime(pil,5));
	D(puts("\n"));

	ip++;
  }
}
