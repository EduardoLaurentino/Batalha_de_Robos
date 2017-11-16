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
  int i, j;
  a = (Arena *) malloc(sizeof(Arena));
  if (!a) Fatal("Memória insuficiente",4);

  a->tempo = 0;
  a->topo_ex = 0;
  a->topo_reg = 0;

  //vetor de ponteiros
  a->celulas = (Celula**)malloc(100 * sizeof(Celula*));
  if (!a->celulas) Fatal("Memória insuficiente",4);
  for (i = 0; i < 100; i++){
    //aloca um vetor de Celulas para cada posição do vetor de ponteiros
    a->celulas[i] = (Celula*) malloc(100 * sizeof(Celula));
      //percorre o vetor de Celulas atual, determinando caracteristicas de cada uma
      for (j = 0; j < 100; j++) {
        a->celulas[i][j].x = i;
        a->celulas[i][j].y = j;
        a->celulas[i][j].ocupado = 0; //ocupado = 0 significa sem ocupacao
        a->celulas[i][j].base = -1; //base = -1 significa sem base; base = 0 time = 0; base = 1 time = 1
      }
  }

  for (i = 0; i < 20; i++) {
    for (j = 0; j < 20; j++) {
      a->celulas[i][j].terreno = 1;
      a->celulas[i][20+j].terreno = 3;
      a->celulas[i][40+j].terreno = 4;
      a->celulas[i][60+j].terreno = 1;
      a->celulas[i][80+j].terreno = 1;

      a->celulas[20+i][j].terreno = 3;
      a->celulas[20+i][20+j].terreno = 3;
      a->celulas[20+i][40+j].terreno = 1;
      a->celulas[20+i][60+j].terreno = 2;
      a->celulas[20+i][80+j].terreno = 1;

      a->celulas[40+i][j].terreno = 4;
      a->celulas[40+i][20+j].terreno = 1;
      a->celulas[40+i][40+j].terreno = 2;
      a->celulas[40+i][60+j].terreno = 1;
      a->celulas[40+i][80+j].terreno = 4;

      a->celulas[60+i][j].terreno = 1;
      a->celulas[60+i][20+j].terreno = 2;
      a->celulas[60+i][40+j].terreno = 1;
      a->celulas[60+i][60+j].terreno = 3;
      a->celulas[60+i][80+j].terreno = 3;

      a->celulas[80+i][j].terreno = 1;
      a->celulas[80+i][20+j].terreno = 1;
      a->celulas[80+i][40+j].terreno = 4;
      a->celulas[80+i][60+j].terreno = 3;
      a->celulas[80+i][80+j].terreno = 1;
    }
  }

  //cria 20 cristais
  srand(time(NULL));
  for (i = 0; i < 20; i++) {
      int num1 = rand() % 99;
      int num2 = rand() % 99;

      //se a celula ja tiver cristais, so adiciona mais
      switch (a->celulas[num1][num2].terreno) {
        case 0:
          a->celulas[num1][num2].cristais += 1;
          break;
        case 1:
          a->celulas[num1][num2].cristais += 2;
          break;
        case 2:
          a->celulas[num1][num2].cristais += 5;
          break;
        case 3:
          a->celulas[num1][num2].cristais += 4;
          break;
        case 4:
          a->celulas[num1][num2].cristais += 3;
          break;
      }
  }
  // o comando abaixo envia para o arquivo em python o tipo de terreno e a quantidade de cristais
  // de cada célula da arena
  for(i = 0; i < 100; i++){
    for(j = 0; j < 100; j++){
      fprintf(display, "cel %d %d %d %d\n", i, j, a->celulas[i][j].terreno, a->celulas[i][j].cristais);
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
  int i;
  int j;
  //ativa exercito
  exercitos[topo_ex].ativo = 1;

  //o local  da base sera sempre (4,4) para o "Exercito 0" e (96,96) para o "Exercito 1"
  if(topo_ex == 0){
    exercitos[topo_ex].pos_celula_base[0] = 4;
    exercitos[topo_ex].pos_celula_base[1] = 4;
    celulas[4][4].base = 0;
  }else{
    exercitos[topo_ex].pos_celula_base[0] = 96;
    exercitos[topo_ex].pos_celula_base[1] = 96;
    celulas[96][96].base = 1;
  }

  //registra na celula o numero do exercito que tem base ali
  celulas[x][y].base = topo_ex;

  //a posicao dos robos do exercito no vetor de registros
  //e' colocado no vetor de robos do exercito
  for (i = 0; i < 3; i++) {
    exercitos[topo_ex].robos[i] = (topo_ex*3) + i;
    //coloca em cada robo o numero do seu proprio exercito e do registro na arena
    registros[exercitos[topo_ex].robos[i]]->exercito = topo_ex;
    registros[exercitos[topo_ex].robos[i]]->registro = (topo_ex*3) + i;

    //coloca na maquina qual a posicao dela na arena
    registros[exercitos[topo_ex].robos[i]]->pos[0] = exercitos[topo_ex].pos_celula_base[0]+i+1;
    registros[exercitos[topo_ex].robos[i]]->pos[1] = exercitos[topo_ex].pos_celula_base[1];

    //ativa ocupacao da celula
    celulas[exercitos[topo_ex].pos_celula_base[0]+i+1][exercitos[topo_ex].pos_celula_base[1]].ocupado = 1;

    //coloca na celula a info da maquina que esta ocupando o local
    celulas[exercitos[topo_ex].pos_celula_base[0]+i+1][exercitos[topo_ex].pos_celula_base[1]].maquina_no_local = (topo_ex*3) + i;
    for(j = 0, j < 6; j++) {
      exercitos[topo_ex].robos[i]->MEM[j] = reconheceVizinhaça(exercitos[topo_ex].robos[i], j);
    }
  }

  // os comandos abaixo irão mandar para o arquivo em python as coordenadas da base do exército em questão,
  // a imagem do robô associado a este exército e as posições de todos os robôs
  fprintf(display, "base %d %d %d\n", topo_ex, exercitos[topo_ex].pos_celula_base[0], exercitos[topo_ex].pos_celula_base[1]);
  
  for(i = 0; i < 3; i++){
    if(topo_ex == 0){
      fprintf(display, "rob GILEAD_A.png\n");
      fprintf(display, "robo %d 0 %d %d\n", exercitos[0].robos[i], registros[exercitos[0].robos[i]]->pos[0], registros[exercitos[0].robos[i]]->pos[1]);
    }else{
      fprintf(display, "rob GILEAD_B.png\n");
      fprintf(display, "robo %d 1 %d %d\n", exercitos[1].robos[i], registros[exercitos[1].robos[i]]->pos[0], registros[exercitos[1].robos[i]]->pos[1]);
    }
  }
  topo_ex++;
}

OPERANDO reconheceVizinhaça(Maquina* m, int j) { //Armazena na memoria[0-5] as celulas da vizinhança
  switch(j){
    case 0:                                     // Posicao Nordeste
        if(m->pos[0] == 0 || m->pos[1] == 100) return 0;
        return celulas[m->pos[0] + 1][m->pos[1] + 1];
        break;
    case 1:                                    // Posicao Leste
        if(m->pos[1] == 100) return 0;
        return celulas[m->pos[0]][m->pos[1] + 1];
        break;
    case 2:                                     // Posicao Sudeste
        if(m->pos[0] == 100 || m->pos[1] == 100) return 0;
        return celulas[m->pos[0] - 1][m->pos[1] + 1];
        break;
    case 3:                                     // Posicao Sudoeste
        if(m->pos[0] == 100 || m->pos[1] == 0) return 0;
        return celulas[m->pos[0] - 1][m->pos[1] - 1];
        break;
    case 4:                                     // Posicao Oeste
        if(m->pos[1] == 0) return 0;
        return celulas[m->pos[0]][m->pos[1] - 1];
        break;
    case 5:                                     // Posicao Noroeste
        if(m->pos[0] == 0 || m->pos[1] == 0) return 0;
        return celulas[m->pos[0] + 1][m->pos[1] - 1];
        break;
  }
}

void RemoveExercito(Arena *a, int num_ex) {
  int i;
  //desativa exercito
  exercitos[num_ex].ativo = 0;

  //retira a base
  celulas[exercitos[num_ex].pos_celula_base[0]][exercitos[num_ex].pos_celula_base[1]].base = -1;

  int xbase = exercitos[num_ex].pos_celula_base[0];
  int ybase = exercitos[num_ex].pos_celula_base[1];
  fprintf(display, "cel %d %d %d %d\n", xbase, ybase, a->celulas[xbase][ybase].terreno, a->celulas[xbase][ybase].cristais);

  for (i = 0; i < 3; i++)
    //retira a ocupacao das celulas onde os robos do exercito estavam
    celulas[registros[exercitos[num_ex].robos[i]]->pos[0]][registros[exercitos[num_ex].robos[i]]->pos[1]].ocupado = 0;

    int x = registros[exercitos[num_ex].robos[i]]->pos[0];
    int y = registros[exercitos[num_ex].robos[i]]->pos[1];
    fprintf(display, "cel %d %d %d %d\n", x, y, a->celulas[x][y].terreno, a->celulas[x][y].cristais);
}

void destroi_arena(Arena *a) {
  free(celulas);
  free(a);
  fprintf("fim\n");
}

//Verifica se existe pelo menos 1 robo de 1 exercito vivo:
int verifica_exercito_ativo(Exercito exerc){
  int XcoordBaseExerc = exerc.pos_celula_base[0]; // coordenada X da base do exercito em questao
  int YcoordBaseExerc = exerc.pos_celula_base[1]; // coordenada Y da base do exercito em questao
  if(celulas[XcoordBaseExerc][YcoordBaseExerc].cristais >= 5) return 0; //Se a base do exercito em questao tiver 5 cristais, jogo acaba
  int i;
  int cont;
  for(i = 0; i < 3; i++){                    //varredura de todos os robos ate encontrar pelo menos 1 com saude > 0.
    if(registros[exerc.robos[i]]->saude > 0){
      cont = 1;
    }else{
      celulas[registros[exerc.robos[i]]->pos[0]][registros[exerc.robos[i]]->pos[1]].ocupado = 0; //A celula em que o robo morreu se torna desocupada (desocupado=0)
      
      int x = registros[exercitos[num_ex].robos[i]]->pos[0];
      int y = registros[exercitos[num_ex].robos[i]]->pos[1];
      fprintf(display, "cel %d %d %d %d\n", x, y, a->celulas[x][y].terreno, a->celulas[x][y].cristais);
    }
  }
  if(cont == 1) return 1;                     //caso haja 1 robo vivo, return 1 = "ativo";
  else return 0;                              //caso nao haja nenhum robo com vida (energia = 0), retornar 0 = "inativo".
}

int verifica_continuidade(){                                      //não continua se excedeu o numero maximo de rodadas.
    int i;
    int quant = 0;
    int exerc_vencedor = -1;
    for(i = 0; i < 2; i++){   //dois exercitos apenas
        exercitos[i].ativo = verifica_exercito_ativo(exercitos[i]); //verificar se existem exercitos ativos.
        if (exercitos[i].ativo == 1) {quant++; exerc_vencedor = i;}   //guarda o numero do exercito que ta ativo.
    }
    if (quant == 2) return -1;                                       //se os dois exercitos estao ativos, o jogo continua(retorna -1)
    else return exerc_vencedor;                                     //caso haja apenas 1, retorna o numero do unico exercito ativo(vencedor)
}


void escalonador(Arena *a, int quant_rod){
    int i, j;
    int verifica_cont;
    for(i = 0; i < quant_rod; i++){
      verifica_cont = verifica_continuidade();//funcao "verifica_continuidade()" retorna -1: caso o jogo continue; 0: caso exerc 0 vença e 1: caso o exerc 1 vença.
      if(verifica_cont < 0){
        for(j = 0; j < 6; j++){ //faz todas os robos executarem 10 instruções por rodadas
          if(registros[j]->isCiclo == 0) exec_maquina(registros[j], 10);
          else                           registro[j]->isCiclo -= 1;
        }
      }else{
        printf("Vencedor: Exército %d\n",verifica_cont); //Printa o exercito vencedor.
        break;                                                   //Jogo acaba.
      }
    }
}


/*===============================================*/

int verifica_ocupacao(int x, int y){ //verifica disponibilidade de conquista da célula (se existir)
  if (x < 0 || y < 0 || x >= 100 || y >= 100) return 1; //indisponibilidade ~= estar ocupado
  else return celulas[x][y].ocupado; //verificar sintáxe
}

int celula_existe(int x, int y){
  if (x < 0 || y < 0 || x >= 100 || y >= 100) return 0; //indisponibilidade ~= não atacavel, não pode atacar e nao perde energia
  else return 1;
}

int retira_energia_movimento(Maquina *m, Terreno terreno){
  switch(terreno){
    case ESTRADA: //Consegue se movimentar no seu proprio turno, pois ESTRADA é um terreno de facil movimentação.
      if(m->energia >= 10) {m->energia -= 10; return 1;}
      else return 0;
      break;
    case TERRA:
      m->isCiclo == 1; //Demora apenas 1 turno para se movimentar caso o terreno seja do tipo TERRA
      if(m->energia >= 15) {m->energia -= 15; return 1;}
      else return 0;
      break;
    case LAMA:
      m->isCiclo == 1; //Demora apenas 1 turno para se movimentar caso o terreno seja do tipo LAMA
      if(m->energia >= 20) {m->energia -= 20; return 1;}
      else return 0;
      break;
    case AGUA:
      m->isCiclo == 2; //Demora 2 turnos para se movimentar caso o terreno seja do tipo AGUA
      if(m->energia >= 25) {m->energia -= 25; return 1;}
      else return 0;
      break;
    case MONTANHA:
      m->isCiclo == 2; //Demora 2 turnos para se movimentar caso o terreno seja do tipo MONTANHA
      if(m->energia >= 30) {m->energia -= 30; return 1;}
      else return 0;
      break;
  }
}

int retira_energia_extracao_e_por(Maquina *m, Terreno terreno){
    switch(terreno){
      case AGUA: //Consegue retirar cristal no seu proprio turno, pois AGUA é um terreno facil de se extrair.
        if(m->energia >= 5 + 20) {m->energia -= 5; return 1;}
        else return 0;
        break;
      case LAMA: 
        m->isCiclo == 1; //Demora apenas 1 turno para retirar cristal caso o terreno seja do tipo LAMA
        if(m->energia >= 7 + 20) {m->energia -= 7; return 1;}
        else return 0;
        break;
      case TERRA: 
        m->isCiclo == 1; //Demora apenas 1 turno para retirar cristal caso o terreno seja do tipo TERRA
        if(m->energia >= 9 + 20) {m->energia -= 9; return 1;}
        else return 0;
        break;
      case ESTRADA: 
        m->isCiclo == 2; //Demora 2 turnos para retirar cristal caso o terreno seja do tipo ESTRADA
        if(m->energia >= 12 + 20) {m->energia -= 12; return 1;}
        else return 0;
        break;
      case MONTANHA: 
        m->isCiclo == 2; //Demora 2 turnos para retirar cristal caso o terreno seja do tipo MONTANHA
        if(m->energia >= 15 + 20) {m->energia -= 15; return 1;}
        else return 0;
        break;
  }
}

#define x (m->pos[0])
#define y (m->pos[1])

#define aqui 0
#define norte 1
#define nordeste 2
#define sudeste 3
#define sul 3
#define sudoeste 4
#define noroeste 5

int movimentacao(Maquina *m, int i, int j){
  if (verifica_ocupacao(i, j) == 0 && retira_energia_movimento(m, celulas[i][j].terreno) == 1) { //verifica se a célula para a qual quer ir existe e esta vazia e se o robo tem energia para ir, já subtraindo energia caso sim
    celulas[x][y].ocupado = 0; //muda o status da celula onde tava para desocupada
    fprintf(display, "cel %d %d %d %d\n", x, y, a->celulas[x][y].terreno, a->celulas[x][y].cristais);
    m->pos[0] = i;
    m->pos[1] = j; //atualiza posicao robo
    celulas[x][y].maquina_no_local = m->registro;
    celulas[x][y].ocupado = 1;
    if(m->exercito == 0){
      fprintf(display, "rob GILEAD_A.png\n");
      fprintf(display, "robo %d 0 %d %d\n", m->registro, m->pos[0], m->pos[1]);
    }else{
      fprintf(display, "rob GILEAD_B.png\n");
      fprintf(display, "robo %d 1 %d %d\n", m->registro, m->pos[0], m->pos[1]);
    }
    switch(terreno){
      case ESTRADA: //Consegue se movimentar no seu proprio turno, pois ESTRADA é um terreno de facil movimentação.
        break;
      case TERRA:
        m->isCiclo == 1; //Demora apenas 1 turno para se movimentar caso o terreno seja do tipo TERRA
        break;
      case LAMA:
        m->isCiclo == 1; //Demora apenas 1 turno para se movimentar caso o terreno seja do tipo LAMA
        break;
      case AGUA:
        m->isCiclo == 2; //Demora 2 turnos para se movimentar caso o terreno seja do tipo AGUA
      case MONTANHA:
        m->isCiclo == 2; //Demora 2 turnos para se movimentar caso o terreno seja do tipo MONTANHA
        break;
    }
    return 1;
  }
  else return 0;
}

int extracao(Maquina *m, int i, int j){
  if (verifica_ocupacao(i, j) == 0 && retira_energia_extracao_e_por(m, celulas[i][j].terreno) == 1 && celulas[i][j].cristais > 0 && m->cristais < 3){ //verifica se a célula para a qual quer ir existe e esta vazia, se sim tenta extrair (e nisso ja perde energia) mas só consegue se tiver cristais. não pode extrair se tiver mais de dois já
    celulas[i][j].cristais -= 1;
    m->cristais += 1;
    m->energia -= 20; //perda de energia maior por fazer extração em célula vizinha (fazer extração em céula vizinha ao invés de onde sestá tem custo energético maior);
    switch(celulas[i][j].terreno){
      case AGUA: //Consegue retirar cristal no seu proprio turno, pois AGUA é um terreno facil de se extrair.
        break;
      case LAMA: 
        m->isCiclo == 1; //Demora apenas 1 turno para retirar cristal caso o terreno seja do tipo LAMA
        break;
      case TERRA: 
        m->isCiclo == 1; //Demora apenas 1 turno para retirar cristal caso o terreno seja do tipo TERRA
        break;
      case ESTRADA: 
        m->isCiclo == 2; //Demora 2 turnos para retirar cristal caso o terreno seja do tipo ESTRADA
        break;
      case MONTANHA: 
        m->isCiclo == 2; //Demora 2 turnos para retirar cristal caso o terreno seja do tipo MONTANHA
        break;
    }
    fprintf(display, "cel %d %d %d %d\n", x, y, a->celulas[i][j].terreno, a->celulas[i][j].cristais);
    if(i == m->pos[0] && j == m->pos[1]){
      if(m->exercito == 0){
        fprintf(display, "rob GILEAD_A.png\n");
        fprintf(display, "robo %d 0 %d %d\n", m->registro, m->pos[0], m->pos[1]);
      }else{
        fprintf(display, "rob GILEAD_B.png\n");
        fprintf(display, "robo %d 1 %d %d\n", m->registro, m->pos[0], m->pos[1]);
      }
    }
    return 1;
  }
  else return 0;
}

int por_cristal(Maquina *m, int i, int j){
  if (verifica_ocupacao(i, j) == 0 && retira_energia_extracao_e_por(m, celulas[i][j].terreno) == 1 && m->cristais > 0) { //verifica se a célula para a qual quer ir existe e esta vazia e tbm se tem cristais na maquina
    celulas[i][j].cristais += 1;
    m->cristais -= 1;
    m->energia -= 20;
    fprintf(display, "cel %d %d %d %d\n", x, y, a->celulas[i][j].terreno, a->celulas[i][j].cristais);
  }else{
    return 0;
  }
}

int atacar(Maquina *m, int i, int j, int n){
  int perde;
  if(n == 1) m->isCiclo == 1, perde = 3; //Caso nao esteja em um ciclo e escolher a arma tipo 1(a mais forte), ele fica ocupado por 1 turno.
  //                          se a arma(n) for a do tipo 1, o oponente perde 'arma[3]' de saude.
  if(n == 0) perde = 1; //se a arma(n) for a do tipo 0, o oponente perde 'arma[1]' de saude
  if (celula_existe(i, j) == 1 && m->energia >= arma[perde - 1]){
    if(celulas[i][j].ocupado == 1){ //tem robo pra ser atacado
      registros[celulas[i][j].maquina_no_local]->saude -= arma[perde];//retura energia do robo atacado;tacado
      m->energia -= arma[perde - 1];//perde energia por atacar com sucesso
      return 1;
    }
    else{
      m->energia -= arma[perde - 1]; //perde energia simplesmente por atacar, pra impedir que fique atacando descreteriosamente
      return 0; //não atacou
    }
  }
  else return 0;
}

int Sistema(OPERANDO op, Maquina *m){
  int dir = op.valor;

  if (op.t == MOV){
      if (dir == aqui){ //é a operacao de descanso, que recarrega um pouco de energia
          if (m->energia < 800 && m->energia > 0) {m->energia += 20; return 1;} //aumenta a energia e retorna o sucesso do procedimento; só pode aumentar energia se ainda estiver vivo (energia > 0)
          return 0; //se não conseguir aumentar energia (uma vez que ficou com menos de 1000, nao pode voltar a ter 1000;
      }
      else if (dir == leste)    return movimentacao(m, x, y + 1);
      else if (dir == nordeste) return movimentacao(m, x-1, y+1);
      else if (dir == sudeste)  return movimentacao(m, x, y+1);
      else if (dir == oeste)      return movimentacao(m, x, y - 1);
      else if (dir == sudoeste) return movimentacao(m, x, y-1);
      else if (dir == noroeste) return movimentacao(m, x-1, y-1);
  }

  else if (op.t == EXTR){
      if (dir == aqui){
            if (retira_energia_extracao_e_por(m, celulas[x][y].terreno) == 1 && celulas[x][y].cristais > 0) { //só de tentar extrair já perde energia, mas só vai conseguir se tiver cristais disponiveis
              celulas[x][y].cristais -= 1;
              m->cristais += 1;
              return 1;
            }
            else return 0;
      }
      else if (dir == leste)    return extracao(m, x, y + 1);
      else if (dir == nordeste) return extracao(m, x-1, y+1);
      else if (dir == sudeste)  return extracao(m, x, y+1);
      else if (dir == oeste)      return extracao(m, x, y - 1);
      else if (dir == sudoeste) return extracao(m, x, y-1);
      else if (dir == noroeste) return extracao(m, x-1, y-1);
  }

  else if (op.t == POR){
      if (dir == aqui){
            if (retira_energia_extracao_e_por(m, celulas[x][y].terreno) == 1 && m->cristais > 0) { //verifica se tem cristais pra por e só coloca se tiver energia, mas perde energia só de tentar
              celulas[x][y].cristais += 1;
              m->cristais -= 1;
              return 1;
            }
            else return 0;
      }
      else if (dir == leste)    return por_cristal(m, x, y + 1);
      else if (dir == nordeste) return por_cristal(m, x-1, y+1);
      else if (dir == sudeste)  return por_cristal(m, x, y+1);
      else if (dir == oeste)      return por_cristal(m, x, y - 1);
      else if (dir == sudoeste) return por_cristal(m, x, y-1);
      else if (dir == noroeste) return por_cristal(m, x-1, y-1);
  }

  else if (op.t == ATK){
       if (dir == aqui){
            if(m->energia >= 30){
              m->energia -= 30 + 130;
              return 1;
            }
            return 0; //sem energia suficiente
       }
      else if (dir == leste)    return atacar(m, x, y + 1);
      else if (dir == nordeste) return atacar(m, x-1, y+1);
      else if (dir == sudeste)  return atacar(m, x, y+1);
      else if (dir == oeste)      return atacar(m, x, y - 1);
      else if (dir == sudoeste) return atacar(m, x, y-1);
      else if (dir == noroeste) return atacar(m, x-1, y-1);
  }
} 
int main(int ac, char **av) {
  display = popen("./apres", "w");

  if (display == NULL) {
    fprintf(stderr,"Não encontrei o programa de exibição\n");
    return 1;
  }

  Arena *a = cria_arena();

  //jogador 1 com 3 robos no exercito
  Maquina *maq0 = cria_maquina(programa);
  RegistroMaquina(a, maq0);

  Maquina *maq1 = cria_maquina(programa);
  RegistroMaquina(a, maq1);

  Maquina *maq2 = cria_maquina(programa);
  RegistroMaquina(a, maq2);

  InsereExercito(a);

  //jogador 2 com 3 robos no exercito
  Maquina *maq3 = cria_maquina(programa);
  RegistroMaquina(a, maq3);

  Maquina *maq4 = cria_maquina(programa);
  RegistroMaquina(a, maq4);

  Maquina *maq5 = cria_maquina(programa);
  RegistroMaquina(a, maq5);

  InsereExercito(a);

  RemoveExercito(a, 1);
  RemoveExercito(a, 0);

  testes_remocao(a);

  destroi_maquina(maq0);
  destroi_maquina(maq1);
  destroi_maquina(maq2);
  destroi_maquina(maq3);
  destroi_maquina(maq4);
  destroi_maquina(maq5);

  destroi_arena(a);
  return 0;
}
