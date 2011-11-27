#include "banco.h"




int Banco::criar_banco(char *pnome) {


	char nomearq[MAXSTR];
	pastaarq(nomearq, pnome, pnome, "banco");

	if (FILE *f = fopen(nomearq, "rb")) {
		printf("Ja existe.\n");
		fclose(f);
		return 0;
	}

	strcpy(nome, pnome);
	mudou = false;


	char comando[MAXSTR] = "mkdir ";
	strcat(comando, pnome);

	system(comando);



	//strcpy(nomearq, pnome);
	//strcat(nomearq, "/");
	//strcat(nomearq, pnome);
	//strcat(nomearq, ".banco");
	pastaarq(nomearq, pnome, pnome, "banco");

	FILE *b = fopen(nomearq, "wb");
	if (!b) {
		printf("Nao criou %s\n", nomearq);
		return 0;
	}

	int qtabs = cabs.size();
	fwrite(&qtabs, sizeof(int), 1, b);
	fclose(b);
	return 0;
}


int Banco::abrir_banco(char *pnome) {

	//char nomearq[MAXSTR + 6];
	//strcpy(nomearq, pnome);
	//strcat(nomearq, ".banco");

	char nomearq[MAXSTR];
	pastaarq(nomearq, pnome, pnome, "banco");

	FILE *b = fopen(nomearq, "rb");
	if (!b) {
		printf("Nao abriu %s\n", nomearq);
		return 0;
	}

	mudou = false;

	strcpy(nome, pnome);

	int qtabs;
	fread(&qtabs, sizeof(int), 1, b);

	Header *h;
	char buff[MAXSTR];

	for (int c1; c1 < qtabs; c1++) {
		fread(buff, MAXSTR, 1, b);

		h = new Header(this, buff);
		cabs.push_back(*h);
	}

	fclose(b);
	return 1;
}


void Banco::fechar_banco() {
	if (mudou) {

		//char nomearq[MAXSTR + 6];
		//strcpy(nomearq, nome);
		//strcat(nomearq, ".banco");
		char nomearq[MAXSTR];
		pastaarq(nomearq, nome, nome, "banco");

		FILE *b = fopen(nomearq, "wb");

		if (!b) {
			cout << "nao abriu.\n";
			return;
		}

		int qcabs = cabs.size();
		cout << "adicionando " << qcabs << ".\n";
		fwrite(&qcabs, sizeof(int), 1, b);

		list<Header>::iterator i;
		for (i = cabs.begin(); i != cabs.end(); i++) {
			fwrite(i->cab.nome, MAXSTR, 1, b);
		}
		fclose(b);
	}
}


void Banco::adicionar_tabela(Header h) {

	if (procurar_tabela(h.cab.nome)) return;

	mudou = true;
	cabs.push_back(h);

}


bool Banco::remover_tabela(char *pnome) {


	list<Header>::iterator i;

	for (i = cabs.begin(); i != cabs.end(); i++) {

		if (!strcmp(i->cab.nome, pnome)) {
			cabs.erase(i);
			mudou = true;
			return true;
		}
	}
	return false;
}


Header *Banco::procurar_tabela(char *pnome) {

	list<Header>::iterator i;

	for (i = cabs.begin(); i != cabs.end(); i++) {
		if (!strcmp(i->cab.nome, pnome)) {


			return (Header *) &i;
		}
	}
	return NULL;
}






void remover_banco(char *nome) {
	char buff[MAXSTR] = "rm -r ";
	strcat(buff, nome);
	system(buff);
}



