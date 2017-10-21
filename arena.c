#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arena.h"

static void Erro(char *msg) {
  fprintf(stderr,"%s\n", msg);
}
static void Fatal(char *msg, int cod) {
  Erro(msg);
  exit(cod);
}

Arena *a;

Arena *cria_arena() {
  Arena *a = (Arena*)malloc(sizeof(Arena));
  if (!a) Fatal("Memória insuficiente",4);

  a->tempo = 0;
  a->topo_ex = 0;
  a->topo_reg = 0;

  //vetor de ponteiros
  Celula **celulas = (Celula**)malloc(100 * sizeof(Celula*));
  if (!celulas) Fatal("Memória insuficiente",4);
  for (int i = 0; i < 100; i++){
    //aloca um vetor de Celulas para cada posição do vetor de ponteiros
    celulas[i] = (Celula*) malloc(100 * sizeof(Celula));
      //percorre o vetor de Celulas atual, determinando caracteristicas de cada uma
      for (int j = 0; j < 100; j++) {
        celulas[i][j].x = i;
        celulas[i][j].y = j;
        celulas[i][j].ocupado = 0; //ocupado = 0 significa sem ocupacao
        celulas[i][j].base = -1; //base = -1 significa sem base; base = 0 time = 0; base = 1 time = 1
      }
  }

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 20; j++) {
      celulas[i][j].terreno = 1;
      celulas[i][20+j].terreno = 3;
      celulas[i][40+j].terreno = 4;
      celulas[i][60+j].terreno = 1;
      celulas[i][80+j].terreno = 1;

      celulas[20+i][j].terreno = 3;
      celulas[20+i][20+j].terreno = 3;
      celulas[20+i][40+j].terreno = 1;
      celulas[20+i][60+j].terreno = 2;
      celulas[20+i][80+j].terreno = 1;

      celulas[40+i][j].terreno = 4;
      celulas[40+i][20+j].terreno = 1;
      celulas[40+i][40+j].terreno = 2;
      celulas[40+i][60+j].terreno = 1;
      celulas[40+i][80+j].terreno = 4;

      celulas[60+i][j].terreno = 1;
      celulas[60+i][20+j].terreno = 2;
      celulas[60+i][40+j].terreno = 1;
      celulas[60+i][60+j].terreno = 3;
      celulas[60+i][80+j].terreno = 3;

      celulas[80+i][j].terreno = 1;
      celulas[80+i][20+j].terreno = 1;
      celulas[80+i][40+j].terreno = 4;
      celulas[80+i][60+j].terreno = 3;
      celulas[80+i][80+j].terreno = 1;
    }
  }

  //cria 200 cristais
  srand(time(NULL));
  for (int i = 0; i < 200; i++) {
      int num1 = rand() % 99;
      int num2 = rand() % 99;

      //se a celula ja tiver cristais, so adiciona mais
      switch (celulas[num1][num2].terreno) {
        case 0:
          celulas[num1][num2].cristais += 1;
          break;
        case 1:
          celulas[num1][num2].cristais += 2;
          break;
        case 2:
          celulas[num1][num2].cristais += 5;
          break;
        case 3:
          celulas[num1][num2].cristais += 4;
          break;
        case 4:
          celulas[num1][num2].cristais += 3;
          break;
      }
  }
  return a;
}

#define registros (a->registros)
#define tempo (a->tempo)
#define exercitos (a->exercitos)
#define celulas (a->celulas)
#define topo_ex (a->topo_ex)
#define topo_reg (a->topo_reg)

void RegistroMaquina(Arena *a, Maquina *m) {
  //coloca o endereco da maquina virtual recebida no vetor de registros
    registros[topo_reg++] = m;
}

//insere o novo exercito e cria uma base para ele
void InsereExercito(Arena *a) {
  //ativa exercito
  exercitos[topo_ex].ativo = 1;

  //define o local *aleatorio* da base
  srand(time(NULL));
  int x = rand() % 96;
  int y = rand() % 96;
  exercitos[topo_ex].pos_celula_base[0] = x;
  exercitos[topo_ex].pos_celula_base[1] = y;

  //registra na celula o numero do exercito que tem base ali
  //celulas[x][y].base = topo_ex;

  //a posicao dos robos do exercito no vetor de registros
  //e' colocado no vetor de robos do exercito
  for (int i = 0; i < 3; i++) {
    exercitos[topo_ex].robos[i] = (topo_ex*3) + i;
    //coloca em cada robo o numero do seu proprio exercito e do registro na arena
    registros[exercitos[topo_ex].robos[i]]->exercito = topo_ex;
    registros[exercitos[topo_ex].robos[i]]->registro = i;

    //coloca na maquina qual a posicao dela na arena
    registros[exercitos[topo_ex].robos[i]]->pos[0] = exercitos[topo_ex].pos_celula_base[0]+i+1;
    registros[exercitos[topo_ex].robos[i]]->pos[1] = exercitos[topo_ex].pos_celula_base[1];

    //ativa ocupacao da celula
    //celulas[exercitos[topo_ex].pos_celula_base[0]+i+1][exercitos[topo_ex].pos_celula_base[1]].ocupado = 1;

    //coloca na celula a info da maquina que esta ocupando o local
    //celulas[pos_celula_base[0]+i+1][pos_celula_base[1]].maquina_no_local = i;
  }
  topo_ex++;
}

void RemoveExercito(Arena *a, int num_ex) {
  //desativa exercito
  exercitos[num_ex].ativo = 0;

  //for (int i = 0; i < 3; i++)
    //retira a ocupacao das celulas onde os robos do exercito estavam
    //celulas[registros[exercitos[num_ex].robos[i]]->pos[0]][registros[exercitos[num_ex].robos[i]]->pos[1]].ocupado = 0;

    //retira a base
    //celulas[exercitos[num_ex].pos_celula_base[0]][exercitos[num_ex].pos_celula_base[1]].base = 0;
}



void Atualiza(Arena *a, int quant_rod) {
  while (tempo < quant_rod) {
    if (verifica_continuidade(a) == 0) break;
    for (int i = 0; i < topo_reg; i++) //cada robo executa 50 instrucoes por rodada
      exec_maquina(registros[i], 50);
    tempo++; //quantas rodadas ja foram
  }
}

//Verifica se existe pelo menos 1 robo de 1 exercito vivo
int verifica_exercito_ativo(Arena *a, Exercito exercito) {
  int cont = 0;
  //varredura de todos os robos ate encontrar pelo menos 1 com energia > 0
  for (int i = 0; i < 3; i++)
    if (registros[exercito.robos[i]]->energia > 0) cont++;

  //caso haja 1 robo vivo, return 1 = "ativo"
  if (cont > 0) return 1;
  //caso nao haja nenhum robo com vida (energia = 0), retornar 0 = "inativo"
  else return 0;
}

void destroi_arena(Arena *a) {
  free(celulas);
  free(a);
}

//Verifica se existe pelo menos 1 robo de 1 exercito vivo:
int verifica_exercito_ativo(Exercito exerc){
  int i;
  for(i = 0; i < 3; i++){                         //varredura de todos os robos ate encontrar pelo menos 1 com energia > 0.
    if(exerc->robos[i].energia > 0) return 1;     //caso haja 1 robo vivo, return 1 = "ativo";
  }
  return 0;                                       //caso nao haja nenhum robo com vida (energia = 0), retornar 0 = "inativo".
}

int verifica_continuidade(){                                      //não continua se excedeu o numero maximo de rodadas.
    int i;
    int cont = 0;
    for(i = 0; i < 2; i++){ //dois exercitos apenas
        exercitos[i].ativo = verifica_exercito_ativo(exercitos[i]); //verificar se existem exercitos ativos.
        if (a->exercitos[i]->ativo == 1) cont++;
        if (cont > 1) return 1;                                             //se há pelo menos dois exercitos ativos, continua
    }
    return 0;
}


void escalonador(Arena *a, int quant_rod){
    int i;
    for(i = 0; i < quant_rod; i++){
      if(verifica_continuidade() == 1){
        for(i = 0; i < a->topo_reg; i++){ //faz todas os robos executarem 50 instruções
          exec_maquina(a->registros[i], 50);
          //Atualiza(); //atualiza a arena depois de cada conjunto de ações de cada robo
        }
      }
    }
}


/*===============================================*/

int verifica_ocupacao(int x, int y){ //verifica disponibilidade de conquista da célula (se existir)
  if (x < 0 || y < 0 || x >= 100 || y >= 100) return 1; //indisponibilidade ~= estar ocupado
  else return a->celulas[x][y]->ocupado; //verificar sintáxe
}

int celula_existe(int x, int y){
  if (x < 0 || y < 0 || x >= 100 || y >= 100) return 0; //indisponibilidade ~= não atacavel, não pode atacar e nao perde energia
  else return 1;
}

int retira_energia_movimento(Maquina *m, Terreno terreno){
  switch(terreno){
    case ESTRADA:
      if(m->energia >= 10) {m->energia -= 10; return 1;}
      else return 0;
      break;
    case TERRA:
      if(m->energia >= 15) {m->energia -= 15; return 1;}
      else return 0;
      break;
    case LAMA:
      if(m->energia >= 20) {m->energia -= 20; return 1;}
      else return 0;
      break;
    case AGUA:
      if(m->energia >= 25) {m->energia -= 25; return 1;}
      else return 0;
      break;
    case MONTANHA:
      if(m->energia >= 30) {m->energia -= 30; return 1;}
      else return 0;
      break;
  }
}

int retira_energia_extracao_e_por(Maquina *m, Terreno terreno){
    switch(terreno){
      case AGUA:
        if(m->energia >= 5 + 20) {m->energia -= 5; return 1;}
        else return 0;
        break;
      case LAMA:
        if(m->energia >= 7 + 20) {m->energia -= 7; return 1;}
        else return 0;
        break;
      case TERRA:
        if(m->energia >= 9 + 20) {m->energia -= 9; return 1;}
        else return 0;
        break;
      case ESTRADA:
        if(m->energia >= 12 + 20) {m->energia -= 12; return 1;}
        else return 0;
        break;
      case MONTANHA:
        if(m->energia >= 15 + 20) {m->energia -= 15; return 1;}
        else return 0;
        break;
  }
}

#define x (m->pos[0])
#define y (m->pos[1])

int Sistema(OPERANDO op, Maquina *m){
  int dir = op.valor;
  switch (op.t){
    case MOV:
       switch(dir){
        case aqui: //é a operacao de descanso, que recarrega um pouco de energia
          if (m->energia < 900) {m->energia += 10; return 1;} //aumenta a energia e retorna o sucesso do procedimento
          return 0; //se não conseguir aumentar energia,
        break;

        case norte:
          if (verifica_ocupacao(a, x - 1, y) == 0 && retira_energia_movimento(m, a->celulas[x-1][y]->terreno) == 1) { //verifica se a célula para a qual quer ir existe e esta vazia e se o robo tem energia para ir, já subtraindo energia caso sim
            a->celulas[x][y]->ocupado = 0; //muda o status da celula onde tava para desocupada
            m->pos[0] = x - 1;
            m->pos[1] = y; //atualiza posicao robo
            a->celulas[x][y]->maquina_no_local = &m;
            a->celulas[x][y]->ocupado = 1;
            return 1;
          }
          else return 0;
          break;
        case nordeste:
          if (verifica_ocupacao(a, x - 1, y + 1) == 0 && retira_energia_movimento(m, a->celulas[x-1][y+1]->terreno) == 1) { //verifica se a célula para a qual quer ir existe e esta vazia e se o robo tem energia para ir, já subtraindo energia caso sim
            a->celulas[x][y]->ocupado = 0; //muda o status da celula onde tava para desocupada
            m->pos[0] = x - 1;
            m->pos[1] = y + 1; //atualiza posicao robo
            a->celulas[x][y]->maquina_no_local = &m;
            a->celulas[x][y]->ocupado = 1;
            return 1;
          }
          else return 0;
          break;
        case sudeste:
          if (verifica_ocupacao(a, x, y + 1) == 0 && retira_energia_movimento(m, a->celulas[x][y+1]->terreno) == 1) { //verifica se a célula para a qual quer ir existe e esta vazia e se o robo tem energia para ir, já subtraindo energia caso sim
            a->celulas[x][y]->ocupado = 0; //muda o status da celula onde tava para desocupada
            m->pos[0] = x;
            m->pos[1] = y + 1; //atualiza posicao robo
            a->celulas[x][y]->maquina_no_local = &m;
            a->celulas[x][y]->ocupado = 1;
            return 1;
          }
          else return 0;
          break;
        case sul:
          if (verifica_ocupacao(a, x + 1, y) == 0 && retira_energia_movimento(m, a->celulas[x+1][y]->terreno) == 1) { //verifica se a célula para a qual quer ir existe e esta vazia e se o robo tem energia para ir, já subtraindo energia caso sim
            a->celulas[x][y]->ocupado = 0; //muda o status da celula onde tava para desocupada
            m->pos[0] = x + 1;
            m->pos[1] = y; //atualiza posicao robo
            a->celulas[x][y]->maquina_no_local = &m;
            a->celulas[x][y]->ocupado = 1;
            return 1;
          }
          else return 0;
          break;
        case sudoeste:
          if (verifica_ocupacao(a, x, y - 1) == 0 && retira_energia_movimento(m, a->celulas[x][y-1]->terreno) == 1) { //verifica se a célula para a qual quer ir existe e esta vazia e se o robo tem energia para ir, já subtraindo energia caso sim
            a->celulas[x][y]->ocupado = 0; //muda o status da celula onde tava para desocupada
            m->pos[0] = x;
            m->pos[1] = y - 1; //atualiza posicao robo
            a->celulas[x][y]->maquina_no_local = &m;
            a->celulas[x][y]->ocupado = 1;
            return 1;
          }
          else return 0;
          break;
        case noroeste:
          if (verifica_ocupacao(a, x - 1, y - 1) == 0 && retira_energia_movimento(m, a->celulas[x-1][y-1]->terreno) == 1) { //verifica se a célula para a qual quer ir existe e esta vazia e se o robo tem energia para ir, já subtraindo energia caso sim
            a->celulas[x][y]->ocupado = 0; //muda o status da celula onde tava para desocupada
            m->pos[0] = x - 1;
            m->pos[1] = y - 1; //atualiza posicao robo
            a->celulas[x][y]->maquina_no_local = &m;
            a->celulas[x][y]->ocupado = 1;
            return 1;
          }
          else return 0;
          break;
      }
      break;

    case EXTR:
      switch(dir){
        case aqui:
          if (retira_energia_extracao_e_por(m, a->celulas[x][y]->terreno) == 1 && a->celulas[x][y]->cristais > 0) { //só de tentar extrair já perde energia, mas só vai conseguir se tiver cristais disponiveis
            a->celulas[x][y]->cristais -= 1;
            m->cristais += 1;
            return 1;
          }
          else return 0;
          break;
        case norte:
          if (verifica_ocupacao(a, x - 1, y) == 0 && retira_energia_extracao_e_por(m, a->celulas[x - 1][y]->terreno) == 1 && a->celulas[x-1][y]->cristais > 0) { //verifica se a célula para a qual quer ir existe e esta vazia, se sim tenta extrair (e nisso ja perde energia) mas só consegue se tiver cristais
            a->celulas[x-1][y]->cristais -= 1;
            m->cristais += 1;
            m->energia -= 20; //perda de energia maior por fazer extração em célula vizinha (fazer extração em céula vizinha ao invés de onde de está tem custo energético maior);
            return 1;
          }
          else return 0;
          break;
        case nordeste:
          if (verifica_ocupacao(a, x - 1, y + 1) == 0 && retira_energia_extracao_e_por(m, a->celulas[x - 1][y + 1]->terreno) == 1 && a->celulas[x-1][y+1]->cristais > 0) {
            a->celulas[x-1][y+1]->cristais -= 1;
            m->cristais += 1;
            m->energia -= 20;
            return 1;
          }
          else return 0;
          break;
        case sudeste:
          if (verifica_ocupacao(a, x, y + 1) == 0 && retira_energia_extracao_e_por(m, a->celulas[x][y+1]->terreno) == 1 && a->celulas[x][y+1]->cristais > 0) {
            a->celulas[x][y+1]->cristais -= 1;
            m->cristais += 1;
            m->energia -= 20;
            return 1;
          }
          else return 0;
          break;
        case sul:
          if (verifica_ocupacao(a, x + 1, y) == 0 && retira_energia_extracao_e_por(m, a->celulas[x+1][y]->terreno) == 1 && a->celulas[x + 1][y]->cristais > 0) {
            a->celulas[x + 1][y]->cristais -= 1;
            m->cristais += 1;
            m->energia -= 20;
            return 1;
          }
          else return 0;
          break;
        case sudoeste:
          if (verifica_ocupacao(a, x, y - 1) == 0 && retira_energia_extracao_e_por(m, a->celulas[x][y-1]->terreno) == 1 && a->celulas[x][y-1]->cristais > 0) {
            a->celulas[x][y-1]->cristais -= 1;
            m->cristais += 1;
            m->energia -= 20;
            return 1;
          }
          else 0;
          break;
        case noroeste:
          if (verifica_ocupacao(a, x - 1, y - 1) == 0 && retira_energia_extracao_e_por(m, a->celulas[x-1][y-1]->terreno) == 1 && a->celulas[x - 1][y - 1]->cristais > 0) {
            a->celulas[x - 1][y - 1]->cristais -= 1;
            m->cristais += 1;
            m->energia -= 20;
            return 1;
          }
          else return 0;
          break;
      }
      break;


    case POR:
      switch(dir){
        case aqui:
          if (retira_energia_extracao_e_por(m, a->celulas[x][y]->terreno) == 1 && m->cristais > 0) { //verifica se tem cristais pra por e só coloca se tiver energia, mas perde energia só de tentar
            a->celulas[x][y]->cristais += 1;
            m->cristais -= 1;
            return 1;
          }
          else return 0;
          break;
        case norte:
          if (verifica_ocupacao(a, x - 1, y) == 0 && retira_energia_extracao_e_por(m, a->celulas[x-1][y]->terreno) == 1 && m->cristais > 0) { //verifica se a célula para a qual quer ir existe e esta vazia e tbm se tem cristais na maquina
            a->celulas[x-1][y]->cristais += 1;
            m->cristais -= 1;
            m->energia -= 20;
          }
          else return 0;
          break;
        case nordeste:
          if (verifica_ocupacao(a, x - 1, y + 1) == 0 && retira_energia_extracao_e_por(m, a->celulas[x - 1][y + 1]->terreno) == 1 && m->cristais > 0) {
            a->celulas[x-1][y+1]->cristais += 1;
            m->cristais -= 1;
            m->energia -= 20;
          }
          else return 0;
          break;
        case sudeste:
          if (verifica_ocupacao(a, x, y + 1) == 0 && retira_energia_extracao_e_por(m, a->celulas[x][y+1]->terreno) == 1 && m->cristais > 0) {
            a->celulas[x][y+1]->cristais += 1;
            m->cristais -= 1;
            m->energia -= 20;
          }
          else return 0;
          break;
        case sul:
          if (verifica_ocupacao(a, x + 1, y) == 0 && retira_energia_extracao_e_por(m, a->celulas[x+1][y]->terreno) == 1 && m->cristais > 0) {
            a->celulas[x + 1][y]->cristais += 1;
            m->cristais -= 1;
            m->energia -= 20;
          }
          else return 0;
          break;
        case sudoeste:
          if (verifica_ocupacao(a, x, y - 1) == 0 && retira_energia_extracao_e_por(m, a->celulas[x][y-1]->terreno) == 1 && m->cristais > 0) {
            a->celulas[x][y-1]->cristais += 1;
            m->cristais -= 1;
            m->energia -= 20;
          }
          else return 0;
          break;
        case noroeste:
          if (verifica_ocupacao(a, x - 1, y - 1) == 0 && retira_energia_extracao_e_por(m, a->celulas[x-1][y-1]->terreno) == 1 && m->cristais > 0) {
            a->celulas[x - 1][y - 1]->cristais += 1;
            m->cristais -= 1;
            m->energia -= 20;
          }
          else return 0;
          break;
      }
      break;


    case ATK:
       switch(dir){
        case aqui:
          if(m->energia >= 30){
            m->energia -= 30 + 130;
            return 1;
          }
          return 0; //sem energia suficiente
          break;
        case norte:
          if (celula_existe(x-1, y) == 1 && m->energia >= 30
            if(a->celulas[x-1][y]->ocupado == 1){
              a->celulas->[x-1][y]->maquina_no_local->energia -= 130;//retura energia do robo atacado;tacado
              m->energia -= 30;//perde energia por atacar com sucesso
              return 1;
            }
            else{
              m->energia -= 30; //perde energia simplesmente por atacar, pra impedir que fique atacando descreteriosamente
              return 0; //não atacou
            }
          }
          else return 0;
          break;
        case nordeste:
          if (celula_existe(x-1, y+1) == 1 && m->energia >= 30){
            if(a->celulas[x-1][y+1]->ocupado == 1){
              a->celulas->[x-1][y+1]->maquina_no_local->energia -= 130;//retura energia do robo atacado;tacado
              m->energia -= 30;//perde energia por atacar com sucesso
              return 1;
            }
            else{
              m->energia -= 30; //perde energia simplesmente por atacar, pra impedir que fique atacando descreteriosamente
              return 0; //não atacou
            }
          }
          else return 0;
          break;
        case sudeste:
          if (celula_existe(x, y+1) == 1 && m->energia >= 30){
            if(a->celulas[x][y+1]->ocupado == 1){
              a->celulas->[x][y+1]->maquina_no_local->energia -= 130;//retura energia do robo atacado;tacado
              m->energia -= 30;//perde energia por atacar com sucesso
              return 1;
            }
            else{
              m->energia -= 30; //perde energia simplesmente por atacar, pra impedir que fique atacando descreteriosamente
              return 0; //não atacou
            }
          }
          else return 0;
          break;
        case sul:
          if (celula_existe(x+1, y) == 1 && m->energia >= 30){
            if(a->celulas[x+1][y]->ocupado == 1){
              a->celulas->[x+1][y]->maquina_no_local->energia -= 130;//retura energia do robo atacado;tacado
              m->energia -= 30;//perde energia por atacar com sucesso
              return 1;
            }
            else{
              m->energia -= 30; //perde energia simplesmente por atacar, pra impedir que fique atacando descreteriosamente
              return 0; //não atacou
            }
          }
          else return 0;
          break;
        case sudoeste:
          if (celula_existe(x, y-1) == 1 && m->energia >= 30){
            if(a->celulas[x][y-1]->ocupado == 1){
              a->celulas->[x][y-1]->maquina_no_local->energia -= 130;//retura energia do robo atacado;tacado
              m->energia -= 30;//perde energia por atacar com sucesso
              return 1;
            }
            else{
              m->energia -= 30; //perde energia simplesmente por atacar, pra impedir que fique atacando descreteriosamente
              return 0; //não atacou
            }
          }
          else return 0;
          break;
        case noroeste:
          if (celula_existe(x-1, y-1) == 1 && m->energia >= 30){
            if(a->celulas[x-1][y-1]->ocupado == 1){
              a->celulas->[x-1][y-1]->maquina_no_local->energia -= 130;//retura energia do robo atacado;tacado
              m->energia -= 30;//perde energia por atacar com sucesso
              return 1;
            }
            else{
              m->energia -= 30; //perde energia simplesmente por atacar, pra impedir que fique atacando descreteriosamente
              return 0; //não atacou
            }
          }
          else return 0;
          break;
      }
      break;
  }
}

