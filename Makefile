arena: arena.o maq.o pilha.o

movimento: teste_sistema_e_escalonador.o arena.o maq.o pilha.o

clean:
	rm -f arena *.o
	rm -f movimento *.o