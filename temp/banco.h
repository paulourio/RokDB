/** Controlador geral do banco.
*/


#ifndef BANCO_H
#define BANCO_H


#include "header.h"



class Banco {

	public:

	char nome[MAXSTR];
	int qtabs;
	Header *cabs;
	bool mudou; //se foi adicionada ou removida uma tabela.

	int criar_banco(char *pnome);

	int abrir_banco(char *nome);

	void fechar_banco();
};





#endif
