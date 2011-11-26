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


void testar_maketable();
void testar_opentable();

int main() {

	Banco b;
	b.criar_banco("Guano");
	b.abrir_banco("Guano");

	//testar_maketable();
	testar_opentable();
	return 0;
}


void testar_maketable() {

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
	strcpy(cab.nome, "Tabelalol");



	printf("Nome1: %s\n", cab.nome);
	cab.qcampos = (unsigned int) 2;
	cab.qpags = (unsigned int) 0;

	printf("Nome2: %s\n", cab.nome);


	Header *h = new Header(cab,c1);

}


void testar_opentable() {
	FILE *f = fopen("Tabelalol.header", "rb");
	Header *h = new Header(f);
	printf("Tam %d\n", h->cols[1].tamcampo);
}
