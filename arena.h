#include "pilha.h"

#define MAXMEM 100

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  *int RMV[100]; //registrador de maquinas virtuais
  int ppl_rmv; // Primeira pos Livre do Regist de Maq Virtuais
  Celula celulas[100][100];	//matriz da arena
  DCJ continuidade;

  //INSTR *prog;
  //int ip;
  //int rbp; //novo registrador
} Arena;

typedef struct {
	int terreno;
	int cristais;
	int ocupacao; //esta ocupado ou nao? 
	int base; //isso eh uma base? de quem?
} Celula;

typedef struct {
	int timestep;
	int ExercAtiv[100];
}DCJ; //Determina a Continuidade do Jogo

Arena *cria_arena(/*sem argumentos?*/);

DCJ *cria_continuidade(/**/);

Celula *cria_celula(/*sem argumentos?*/);

void registro(*Maquina);

//void exec_arena(Arena *m, int n);

void destroi_arena(Arena *m);
