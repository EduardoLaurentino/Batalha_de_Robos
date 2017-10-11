#include "pilha.h"

#define MAXMEM 100

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  int ip;
  int rbp; //novo registrador
  //posição do robo na arena:
  int x;
  int y;
  int Cristais;
} Maquina;

//Um dos problemas que precisam ser enfrentados ´e a identifica¸c˜ao do tipo
//de vari´avel que est´a no topo pilha
typedef struct{
    OPERANDO tipo;
    int valor;
} Identificacao;

Maquina *cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);
