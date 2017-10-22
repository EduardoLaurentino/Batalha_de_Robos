#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

INSTR programa[] = {
  {PUSH, {NUM, 3}},
  {PUSH, {NUM, 6}},
  {CALL, {NUM, 5}},
  {PRN,  {NUM, 0}},
  {END,  {NUM, 0}},
  {ADD,  {NUM, 0}},
  {RET,  {NUM, 0}}
};

void testes_arena(Arena *a) {
  printf("TESTES-ARENA\n\n");

  printf("Matriz/Cristais na arena:\n\n");
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      if (a->celulas[i][j].cristais == 0) printf(".");
      else printf("%d", a->celulas[i][j].cristais);
    }
    printf("\n");
  }
  printf("\n");

  printf("Matriz/Terrenos da arena:\n\n");
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      printf("%d", a->celulas[i][j].terreno);
    }
    printf("\n");
  }
  printf("\n");
}

void testes_insercao(Arena *a) {
  printf("Registros/Dados dos robos:\n\n");
  for (int i = 0; i < 6; i++) {
    printf("Robo: %d\n", i);
    printf("Registro: %d\n", a->registros[i]->registro);
    printf("Energia: %d\n", a->registros[i]->energia);
    printf("Cristais: %d\n", a->registros[i]->cristais);
    printf("Posicao: %d ", a->registros[i]->pos[0]);
    printf("%d\n", a->registros[i]->pos[1]);
    printf("Exercito: %d\n\n", a->registros[i]->exercito);
  }
  printf("\n");

  printf("Matriz/Localizacao das bases na arena apos insercao:\n\n");
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      if (a->celulas[i][j].base == -1) printf(".");
      else printf("%d", a->celulas[i][j].base);
    }
    printf("\n");
  }
  printf("\n");

  for (int i = 0; i < 2; i++)
    printf("Coordenadas X: %d Y: %d\n", a->exercitos[i].pos_celula_base[0], a->exercitos[i].pos_celula_base[1]);
  printf("\n");

  printf("Matriz/Localizacao dos robos na arena apos insercao:\n\n");
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      if (a->celulas[i][j].ocupado == 0) printf(".");
      else printf("%d", a->celulas[i][j].ocupado);
    }
    printf("\n");
  }
  printf("\n");

  printf("Dados dos exercitos:\n\n");
  for (int i = 0; i < 2; i++) {
    printf("Ativo: %d\n", a->exercitos[i].ativo);
    printf("Posicao da base: X %d Y %d\n", a->exercitos[i].pos_celula_base[0], a->exercitos[i].pos_celula_base[1]);

    for (int j = 0; j < 3; j++) {
      printf("Numero do exercito nos robos: %d\n", a->registros[a->exercitos[i].robos[j]]->exercito);
    }
  }
  printf("\n");
}

void testes_remocao(Arena *a) {
  printf("Localizacao das bases na arena apos remocao:\n\n");
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      if (a->celulas[i][j].base == -1) printf(".");
      else printf("%d", a->celulas[i][j].base);
    }
    printf("\n");
  }
  printf("\n");

  printf("Localizacao dos robos na arena apos remocao:\n\n");
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      if (a->celulas[i][j].ocupado == 0) printf(".");
      else printf("%d", a->celulas[i][j].ocupado);
    }
    printf("\n");
  }
  printf("\n");
}

int main(int ac, char **av) {
  Arena *a = cria_arena();

  testes_arena(a);
  
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

  testes_insercao(a);

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
