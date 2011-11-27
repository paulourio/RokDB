#include <iostream>
#include <cstring>
#include "header.h"
#include "tipos.h"
#include "banco.h"

#define DEBUG
#ifdef DEBUG
	#define debugmsg(...) printf(__VA_ARGS__)
#else
	#define debugmsg(...)
#endif


void testar_maketable(Banco *b);
void testar_opentable(Banco *b);

int main() {

	Banco b;
	b.criar_banco("Maoe");
	b.abrir_banco("Maoe");

	testar_maketable(&b);
	//testar_opentable(&b);

	b.remover_tabela("Guano");

	b.fechar_banco();

	remover_banco("Maoe");
	return 0;
}


void testar_maketable(Banco *b) {

	coluna c1[2];

	memset(c1[0].nome, 0, MAXSTR);
	strcpy(c1[0].nome, "val1");
	c1[0].tipo = tint;
	c1[0].notnull = false;
	c1[0].unique = false;
	c1[0].tamcampo = 1;

	memset(c1[1].nome, 0, MAXSTR);
	strcpy(c1[1].nome, "val666");
	c1[1].tipo = tstring;
	c1[1].notnull = true;
	c1[1].unique = true;
	c1[1].tamcampo = 10;

	cabecalho cab;

	memset(cab.nome, 0, MAXSTR);
	strcpy(cab.nome, "Guano");


	cab.qcampos = (unsigned int) 2;
	cab.qpags = (unsigned int) 0;


	Header *h = new Header(b, cab, c1);
	b->adicionar_tabela(*h);

}


void testar_opentable(Banco *b) {
	FILE *f = fopen("Guano.header", "rb");
	Header *h = new Header(b, "Guano");
	printf("Tam %d\n", h->cols[1].tamcampo);
}
