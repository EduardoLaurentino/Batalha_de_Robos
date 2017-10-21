#include "maq.h"

#define MAXMEM 100

//cada exercito tem no maximo 3 robos
typedef struct {
  int ativo;
  Maquina *robos[3]; //enderecos dos robos do exercito
  int pos_celula_base[2];
} Exercito;

typedef struct {
  Celula **celulas; //arena
  Exercito exercitos[20]; //20 exercitos no total, maximo de 60 robos
  Maquina *registros;
  int topo_ex; //local do vetor para inserir novos exercitos
  int topo_reg; //local do vetor para inserir novas maquinas
  int tempo;
} Arena;

void RegistroMaquina(Arena *a, Maquina *m);
void InsereExercito(Arena *a);
void RemoveExercito(Arena *a, int num_exercito);

Arena *cria_arena(int tamanho, int quantos_jogadores);
void destroi_arena(Arena *a);

Celula *cria_celula(); //construtor de uma celula

OPERANDO Sistema(Arena *a, OPERANDO op);

int verifica_exercito_ativo(Arena *a, Exercito exerc); // 0 = nenhum robo de um certo exercito esta vivo; 1, caso contrario.

int verifica_continuidade(Arena *a, int max_rod); //"int" para usarmos retornos 0 ou 1, simulando booleana

void escalonador(Arena *a, int quant_rod);