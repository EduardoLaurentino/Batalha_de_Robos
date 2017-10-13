#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

Arena *cria_arena() {
  Arena *a = (Arena*)malloc(sizeof(Arena));
  if (!a) Fatal("Memória insuficiente",4);
  a->tempo = 0;
  a->topo_ex = 0;
  a->topo_reg = 0;

  //INICIA UM MAPA ESPECIFICO
  for (int i = 0; i < 100; i++) {
    for (int j = 0; i < 100; j++) {
      celulas[i][j].x = i;
      celulas[i][j].y = j;
      celulas[i][j].base = 0,
      celulas[i][j].ocupacao = 0,
    }
  }

  /*  terrenos:
  0 = normal
  1 = montanha
  2 = agua
  3 = lama

  cristal: quanto maior o numero, melhor o cristal  */

  for (int i = 0; i < 20; i++) {
    for (int j = 0; i < 20; j++) {
      celulas[i][j].terreno = 0;
      celulas[i][20+j].terreno = 2;
      celulas[i][40+j].terreno = 3;
      celulas[i][60+j].terreno = 0;
      celulas[i][80+j].terreno = 0;

      celulas[20+i][j].terreno = 2;
      celulas[20+i][20+j].terreno = 2;
      celulas[20+i][40+j].terreno = 0;
      celulas[20+i][60+j].terreno = 1;
      celulas[20+i][80+j].terreno = 0;

      celulas[40+i][j].terreno = 3;
      celulas[40+i][20+j].terreno = 0;
      celulas[40+i][40+j].terreno = 1;
      celulas[40+i][60+j].terreno = 0;
      celulas[40+i][80+j].terreno = 3;

      celulas[60+i][j].terreno = 0;
      celulas[60+i][20+j].terreno = 1;
      celulas[60+i][40+j].terreno = 0;
      celulas[60+i][60+j].terreno = 2;
      celulas[60+i][80+j].terreno = 2;

      celulas[80+i][j].terreno = 0;
      celulas[80+i][20+j].terreno = 0;
      celulas[80+i][40+j].terreno = 3;
      celulas[80+i][60+j].terreno = 2;
      celulas[80+i][80+j].terreno = 0;
    }
  }

  //cria 200 cristais
  //talvez random esteja errado \/
  srand(time(NULL));
  for (int i = 0; i < 200; i++) {
      int num1 = rand() % 99;
      int num2 = rand() % 99;

      //se a celula ja tiver cristais, so adiciona mais
      //terreno mais dificil = mais cristais
      switch (celulas[num1][num2].terreno)
        case 0:
          celulas[num1][num2].cristal += 1,
        case 1:
          celulas[num1][num2].cristal += 5,
        case 2:
          celulas[num1][num2].cristal += 2,
        case 3:
          celulas[num1][num2].cristal += 3,
  }
  return a;
}

Celula *cria_celula(/*sem argumentos*/){
	Celula *c = (Celula)malloc(sizeof(Celula));
	if(!c) Fatal("Memória insuficiente!",4);

    return c;
}


void Atualiza(Arena *a){
    int i;
    for(i = 0; i < a->topo_reg; i++){ //faz todas os robos executarem 10 instruções
        exec_maquina(a->registros[i], 50);
    }
    a->tempo++; //avança uma unidade de tempo
}

//insere o novo exercito e cria uma base para ele
void InsereExercito(Arena *a); {
  a->exercitos[topo_ex]->ativo = 1;

  //talvez random esteja errado \/
  a->exercitos[topo_ex]->celula_base = &celula[rand() % 97][rand() % 97]; //define o local da base
  a->exercitos[topo_ex]->celula_base->base = topo_ex; //define a celula com uma base com o numero do exercito

  for (int i = 0; i < 3; i++) {
    a->exercitos[topo_ex].robos[i] = registros[(topo_ex*3) + i];
    a->exercitos[topo_ex].robos[i]->posicao = a->exercitos[topo_ex].celula_base+i+1; //bota na maquina qual a posicao dela na arena
    a->exercitos[topo_ex].robos[i]->posicao->ocupacao = 1; //ativa ocupacao da celula
    a->exercitos[topo_ex].robos[i]->posicao->maquina_no_local = a->exercitos[topo_ex].robos[i]; //bota na celula a info da maquina que esta ocupando o local
  }
}

void RemoveExercito(Arena *a, int num_exercito); {
  a->exercitos[num_exercito]->ativo = 0;
  for (int i = 0; i < 3; i++)
    a->exercitos[num_exercito].robos[i]->posicao->ocupacao = 0; //retira todos os robos do exercito

  a->exercitos[num_exercito]->*celula_base->base = 0; //retira a base
}

void Sistema(Arena *a, INSTR ip){

}

int verifica_continuidade(Arena *a, int max_rod){
    if (a->tempo > max_rod) return 0; //não continua se excedeu o tempo máximo
    int i;
    int cont = 0;
    for(i = 0; i < topo_ex; i++){
        if (a->exercitos[i]->ativo == 1) cont++;
        if (cont > 1) return 1; //se há pelo menos dois exercitos ativos, continua
    }
}

void registro(Arena *a, Maquina *m){
	a->registros[a->topo_reg++] = m;
}

void escalonador(Arena *a, int quant_rod){
    int i;
    for(i = 0; i < quant_rod; i++){
        Atualiza(a);
    }
}

void destroi_arena(Arena *m){
    free(m);
}