Arena *cria_arena(/*falta argumento?*/) {
  Arena *a = (Arena*)malloc(sizeof(Arena));
  if (!a) Fatal("Memória insuficiente",4);
  a->ppl_rmv = 0;
  return m;
}

Celula *cria_celula(/*sem argumentos?*/){
	Celula *c = (Celula)malloc(sizeof(Celula));
	if(!c) Fatal("Mem[oria insuficiente!",4);
	c->terreno = /*aleatorio*/;
	c->cristais = /*aleatorio*/;
	c->ocupacao = 0;
	c->base = /*aleatorio?*/
}


void registro(*Maquina){
	RMV[ppl_rmv++] = Maquina;
}