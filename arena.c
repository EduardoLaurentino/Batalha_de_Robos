#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

Arena *cria_arena(/*falta argumento?*/) {
    Arena *a = (Arena*)malloc(sizeof(Arena));
    if (!a) Fatal("Memória insuficiente",4);
    a->ppl_rmv = 0;
    return m;
}

Celula *cria_celula(/*sem argumentos?*/){
	Celula *c = (Celula)malloc(sizeof(Celula));
	if(!c) Fatal("Memória insuficiente!",4);
	c->terreno = /*aleatorio*/;
	c->cristais = /*aleatorio*/;
	c->ocupacao = 0;
	c->base = /*aleatorio?*/
}

DCJ *cria_continuidade(/*sem argumentos?*/){
    DCJ *dcj = (DCJ)malloc(sizeof(DCJ));
    if(!DCj) Fatal("Memória insuficiente!",4);
    dcj->timestep = 0;
    dcj->NEA = 0;
}

void InsereExercito(Arena *a){
    /*a->continuidade->ExercAtiv[ppl_EA] = ppl_EA++;*/
    /*NAO SEI*/
}

void RemoveExercito(Arena *a){
    /*NAO SEI*/
}

void Sistema(Arena *a, int op){
    /*NAO SEI*/
}

int verifica_continuidade(Arena *a, int max_rod){
    if (a->DCJ->timestep <= max_road && a->DCJ->NEA > 1) return 1; //ainda tem tempo e exercito
    else return 0;
}

void registro(Arena *a, Maquina *m){
	a->RMV[(a->ppl_rmv)++] = *m;
}

void escalonador(Arena *a, int quant_rod){
    int i;
    for(i = 0; i < quant_rod; i++){
        int j;
        for(j = 0; j < a->ppl_rmv; j++){
            /*EXECUTAR 50 INSTRUÇÕES DE a->RMV[j]*/
        }
    }
}

void destroi_arena(Arena *m){
    free(m);
}