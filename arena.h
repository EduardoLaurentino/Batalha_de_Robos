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
} Arena;

typedef struct {
	int terreno;
	int cristais;
	int ocupacao; //esta ocupado ou nao?
	int base; //isso eh uma base? de quem?
} Celula;

typedef struct {
	int timestep;
	//int ExercAtiv[100];
    int NEA; //número de exercitos ativos
} DCJ; //Determina a Continuidade do Jogo

/*-------MÉTODOS---------*/

Arena *cria_arena(/*sem argumentos?*/); //construtor da arena

DCJ *cria_continuidade(/*sem argumentos?*/); //construtor de um DCJ

Celula *cria_celula(/*sem argumentos?*/); //construtor de uma celula;

void Atualiza(Arena *a); //ONDE ESTÁ/O QUE É o tal do método "run"?

void InsereExercito(Arena *a);

void RemoveExercito(Arena *a);

void Sistema(Arena *a, int op);

int verifica_continuidade(Arena *a, int max_rod); //"int" para usarmos retornos 0 ou 1, simulando booleana

void registro(Arena *a, Maquina *m);

void destroi_arena(Arena *m);

void escalonador(Arena *a, int quant_rod);