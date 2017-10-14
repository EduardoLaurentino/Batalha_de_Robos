#include "pilha.h"

#define MAXMEM 100

typedef struct {

  Celula celulas[MAXMEM][MAXMEM]; //arena 100x100
  Exercito exercitos[20]; //20 exercitos no total, maximo de 60 robos
  *Maquina registros[60];
  int topo_ex; //local do vetor para inserir novos exercitos
  int topo_reg; //local do vetor para inserir novas maquinas
  int tempo;
} Arena;

typedef struct {
  int x,
  int y,
  int terreno,
  int cristal,
  int base, //base com o numero do exercito
  int ocupacao,
  *Maquina maquina_no_local; //endereco do robo que esta na celula se ocupacao = 1
} Celula;


//cada exercito tem no maximo 3 robos
typedef struct {
  int ativo;
  *Maquina robos[3]; //enderecos dos robos do exercito
  *Celula celula_base; //endereco da celula com a base do exercito
} Exercito;

/*-------MÉTODOS---------*/

Arena *cria_arena(/*sem argumentos*/); //construtor da arena

Celula *cria_celula(); //construtor de uma celula;

void Atualiza(Arena *a);

void InsereExercito(Arena *a);

void RemoveExercito(Arena *a);

void Sistema(Arena *a, OPERANDO op);

int verifica_continuidade(Arena *a, int max_rod); //"int" para usarmos retornos 0 ou 1, simulando booleana

void registro(Arena *a, Maquina *m);

void destroi_arena(Arena *m);

void escalonador(Arena *a, int quant_rod);