#include "header.h"

Header::Header(Banco *b, char *nome) {
	char nomearq[MAXSTR];
	pastaarq(nomearq, b->nome, nome, "header");
	FILE *a = fopen(nomearq, "rb");

	banco = b;
	fread(&cab, sizeof(cabecalho), 1, a);
	cols = (coluna *) malloc(sizeof(coluna) * cab.qcampos);
	fread(cols, sizeof(coluna), cab.qcampos, a);

	ler_dados();
}


Header::Header(Banco *b, cabecalho pcab, coluna *pcols) {
	banco = b;
	cab = pcab;
	cols = pcols;
	criar_arq();
	qpags = 0;
}

Header::~Header() {
	if (pags) free(pags);
}



void Header::criar_arq() {

	FILE *a;
	char nomearq[MAXSTR];

	pastaarq(nomearq, banco->nome, cab.nome, "header");
	a = fopen(nomearq, "wb");

	if (!a) {
		//printf("Nao conseguiu criar o arquivo.");
		return;
	}

	fwrite(&cab, sizeof(cabecalho), 1, a);
	fwrite(cols, sizeof(coluna), cab.qcampos, a);
	fclose(a);



	pastaarq(nomearq, banco->nome, cab.nome, "dados");
	a = fopen(nomearq, "wb");
	if (!a) return;

	fwrite(&qpags, sizeof(int), 1, a);
	fwrite(pags, get_tam_reg(), qpags, a);
}


size_t Header::get_tam_reg() {
	size_t tot = 0;

	for (int c1 = 0; c1 < cab.qcampos; c1++) {
		switch (cols[c1].tipo) {
			case tint: tot += sizeof(int); break;
			case tfloat: tot += sizeof(float); break;
			case tstring: tot += cols[c1].tamcampo;
		}
	}
	return tot;
}






void Header::ler_dados() {

	char nomearq[MAXSTR];
	pastaarq(nomearq, banco->nome, cab.nome, "dados");

	FILE *a = fopen(nomearq, "rb");
	if (!a) return;
	fread(&qpags, sizeof(int), 1, a);
	pags = malloc(qpags * get_tam_reg());
	fread(pags, get_tam_reg(), qpags, a);
	fclose(a);
}


void Header::insert(void *reg) {
	qpags++;
	pags = realloc(pags, qpags * get_tam_reg());
	memcpy(pags + (qpags - 1) * get_tam_reg(), reg, get_tam_reg());
}

void *Header::make_reg(...) {
	void *reg = malloc(get_tam_reg());
	if (!reg) return NULL;


}
