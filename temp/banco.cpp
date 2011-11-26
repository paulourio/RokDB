#include "banco.h"




int Banco::criar_banco(char *pnome) {

	strcpy(nome, pnome);
	qtabs = 0;
	mudou = false;

	char nomearq[MAXSTR + 6] = "mkdir";

	system("mkdir "



	strcpy(nomearq, pnome);
	strcat(nomearq, "/");
	strcat(nomearq, pnome);
	strcat(nomearq, ".banco");

	FILE *b = fopen(nomearq, "wb");
	if (!b) {
		printf("Nao criou %s\n", nomearq);
		return 0;
	}

	fwrite(&qtabs, sizeof(int), 1, b);
	fclose(b);
	return 0;
}


int Banco::abrir_banco(char *pnome) {

	char nomearq[MAXSTR + 6];
	strcpy(nomearq, pnome);
	strcat(nomearq, ".banco");

	FILE *b = fopen(nomearq, "rb");
	if (!b) {
		printf("Nao abriu %s\n", nomearq);
		return 0;
	}

	mudou = false;

	strcpy(nome, pnome);
	fread(&qtabs, sizeof(int), 1, b);

	cabs = (Header *) malloc(qtabs * sizeof(Header));
	if (!cabs) return 0;

	for (int c1; c1 < qtabs; c1++) {
		fread(cabs[c1].cab.nome, MAXSTR, 1, b);
	}

	fclose(b);
	return 1;
}


void Banco::fechar_banco() {

	if (mudou) {

		char nomearq[MAXSTR + 6];
		strcpy(nomearq, nome);
		strcat(nomearq, ".banco");

		FILE *b = fopen(nomearq, "wb");
		if (!b) return;

		fwrite(&qtabs, sizeof(int), 1, b);


		for (int c1; c1 < qtabs; c1++) {
			fwrite(cabs[c1].cab.nome, MAXSTR, 1, b);
		}
		fclose(b);
	}

	free(cabs);
}
