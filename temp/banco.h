/** Controlador geral do banco.
*/


#ifndef BANCO_H
#define BANCO_H

class Header;

#include <cstdio>
#include <cstdlib>
#include <list>
#include "util.h"
#include "header.h"



class Banco {

	public:

	char nome[MAXSTR];
	list<Header> cabs;
	bool mudou; //se foi adicionada ou removida uma tabela.

	int criar_banco(char *pnome);

	int abrir_banco(char *nome);

	void fechar_banco();

	void adicionar_tabela(Header h);

	bool remover_tabela(char *pnome);

	Header *procurar_tabela(char *pnome);
};


void remover_banco(char *nome);


#endif
