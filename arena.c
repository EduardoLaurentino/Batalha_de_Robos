#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

Arena *cria_arena() {
  Arena *a = (Arena)malloc(sizeof(Arena));
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

// correlacionamos algumas ações de um robo com numeros.
/*
#define mover 1
#define extrair 2
#define depositar 3
#define atacar 4
*/
// fizemos uma correspondencia entre as vizinhancas de uma celula com numeros.
#define aqui 0
#define norte 1
#define nordeste 2
#define sudeste 3
#define sul 4
#define sudoeste 5
#define noroeste 6


Celula *cria_celula(/*sem argumentos*/){
	Celula *c = (Celula)malloc(sizeof(Celula));
	if(!c) Fatal("Memória insuficiente!",4);

  return c;
}

Exercito *cria_exercito(){
  Exercito *e = (Exercito)malloc(sizeof(Exercito));
  if(!e) Fatal("Memória insuficiente!", 4);

  return e;
}

void Atualiza(Arena *a){
    int i;

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

int verifica_ocupacao(Arena *a, int x, int y){ //verifica disponibilidade de conquista da célula (se existir)
  if (x < 0 || y < 0 || x >= 100 || y >= 100) return 1; //indisponibilidade ~= estar ocupado
  else return a.celulas[x][y]->ocupacao; //verificar sintáxe
}

void atualiza_posicao(Maquina *m, Arena *a, int x, int y)

#define x (m->posicao->x)
#define y (m->posicao->y)
//#define endereco_robo (m->posicao)

void Sistema(Arena *a, OPERANDO op, Maquina *m){
  int dir = op.valor;
  switch (op.t){
    case MOV:
       switch(dir){
        case aqui:
        // nao faz sentido se mover pra mesma celula.
        break;
        case norte:
          if (verifica_ocupacao(a, x - 1, y) == 0) { //verifica se a célula para a qual quer ir existe e esta vazia
            m->posicao = &celulas[x - 1][y]; //guarda o endereço da nova célula na máquina
            m->posicao->ocupacao = 0; //muda o status da celula onde tava para desocupada
            //m->posicao->maquina_no_local = null;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
        break;
        case nordeste:
          if (verifica_ocupacao(a, x - 1, y + 1) == 0) {
            m->posicao = &celulas[x - 1][y + 1];
            m->posicao->ocupacao = 0;
            //m->posicao->maquina_no_local = null;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case sudeste:
          if (verifica_ocupacao(a, x, y + 1) == 0) {
            m->posicao = &celulas[x][y + 1];
            m->posicao->ocupacao = 0;
            //m->posicao->maquina_no_local = null;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case sul:
          if (verifica_ocupacao(a, x + 1, y) == 0) {
            m->posicao = &celulas[x + 1][y];
            m->posicao->ocupacao = 0;
            //m->posicao->maquina_no_local = null;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case sudoeste:
          if (verifica_ocupacao(a, x, y - 1) == 0) {
            m->posicao = &celulas[x][y - 1];
            m->posicao->ocupacao = 0;
            //m->posicao->maquina_no_local = null;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case noroeste:
          if (verifica_ocupacao(a, x - 1, y - 1) == 0) {
            m->posicao = &celulas[x - 1][y - 1];
            m->posicao->ocupacao = 0;
            //m->posicao->maquina_no_local = null;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
      }
      break;
    case EXTR:
      switch(dir){
        case aqui:
        ...
        break;
        case norte:
          if (verifica_ocupacao(a, x - 1, y) == 0 && a->celulas[x-1][y]->cristais > 0) { //verifica se a célula para a qual quer ir existe e esta vazia e tbm se tem cristais
            a->celulas[x-1][y]->cristais -= 1;
            m->cristais += 1;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case nordeste:
          if (verifica_ocupacao(a, x - 1, y + 1) == 0 && a->celulas[x-1][y+1]->cristais > 0) {
            a->celulas[x-1][y+1]->cristais -= 1;
            m->cristais += 1;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case sudeste:
          if (verifica_ocupacao(a, x, y + 1) == 0 && a->celulas[x][y+1]->cristais > 0) {
            a->celulas[x][y+1]->cristais -= 1;
            m->cristais += 1;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case sul:
          if (verifica_ocupacao(a, x + 1, y) == 0 && a->celulas[x + 1][y]->cristais > 0) {
            a->celulas[x + 1][y]->cristais -= 1;
            m->cristais += 1;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case sudoeste:
          if (verifica_ocupacao(a, x, y - 1) == 0 && a->celulas[x][y-1]->cristais > 0) {
            a->celulas[x][y-1]->cristais -= 1;
            m->cristais += 1;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
        case noroeste:
          if (verifica_ocupacao(a, x - 1, y - 1) == 0 && a->celulas[x - 1][y - 1]->cristais > 0) {
            a->celulas[x - 1][y - 1]->cristais -= 1;
            m->cristais += 1;
          }
          else{
            //deveria sinalizar de alguma maneira que não é possível fazer a mudança de local?????
          }
          break;
      }
      break;
    case POR:
       switch(dir){
        case aqui:
        ...
        break;
        case norte:
        ...
        break;
        case nordeste:
        ...
        break;
        case sudeste:
        ...
        break;
        case sul:
        ...
        break;
        case sudoeste:
        ...
        break;
        case noroeste:
        ...
        break;
      }
      break;
    case ATK:
       switch(dir){
        case aqui:
        ...
        break;
        case norte:
        ...
        break;
        case nordeste:
        ...
        break;
        case sudeste:
        ...
        break;
        case sul:
        ...
        break;
        case sudoeste:
        ...
        break;
        case noroeste:
        ...
        break;
      }
      break;
  }
}

int verifica_continuidade(Arena *a, int max_rod){
    if (a->tempo > max_rod) return 0; //não continua se excedeu o tempo máximo
    int i;
    int cont = 0;
    for(i = 0; i < topo_ex; i++){
        if (a->exercitos[i]->ativo == 1) cont++;
        if (cont > 1) return 1; //se há pelo menos dois exercitos ativos, continua
    }
    return 0;
}

void registro(Arena *a, Maquina *m){
	a->registros[a->topo_reg++] = m;
}

void escalonador(Arena *a, int quant_rod){
    int i;
    for(i = 0; i < quant_rod; i++){
        for(i = 0; i < a->topo_reg; i++){ //faz todas os robos executarem 50 instruções
          exec_maquina(a->registros[i], 50);
          Atualiza(); //atualiza a arena depois de cada conjunto de ações de cada robo
        }
    }
}

void destroi_arena(Arena *a){
  free(a);
}

void destroi_celula(Celula *c){
  free(c);
}

void destroi_exercito(Exercito *e){
  free(e);
}