#include "header.h"

Header::Header(FILE *a) {

	fread(&cab, sizeof(cabecalho), 1, a);
	cols = (coluna *) malloc(sizeof(coluna) * cab.qcampos);
	fread(cols, sizeof(coluna), cab.qcampos, a);
}


Header::Header(cabecalho pcab, coluna *pcols) {
	cab = pcab;
	cols = pcols;
	criar_arq();
}

Header::~Header() {
	// ( :-D )
}



void Header::criar_arq() {

	FILE *a;
	char nomearq[MAXSTR + 7];
	strcpy(nomearq, cab.nome);
	strcat(nomearq, ".header");

	printf("Nome: %s\n", nomearq);
	a = fopen(nomearq, "wb");

	if (!a) {
		printf("Nao conseguiu criar o arquivo.");
		return;
	}

	fwrite(&cab, sizeof(cabecalho), 1, a);
	fwrite(cols, sizeof(coluna), cab.qcampos, a);

	fclose(a);
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




int Header::regs_por_pag() {
	return TAMPAGUSO / get_tam_reg();
}






