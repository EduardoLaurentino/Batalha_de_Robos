#include "pilha.h"

#define MAXMEM 100

typedef struct {
  Pilha* pil;
  Pilha* exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  OPERANDO ip;
  OPERANDO rbp;
  int cristais;
  int pos[2]; //pos[0] = coordenada x na matriz; pos[1] = coordenada y na matriz;
  int energia; //começará com 1000 pontos de energia
  int exercito;
  //int registro; //numero de registro na arena
} Maquina;

Maquina *cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);
