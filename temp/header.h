/**

20/set/2011

Controla o arquivo de header das tabelas.


Formato do arquivo de header (strings 0-finalized):

-Nome da tabela. (string).
-n = Quantidade de campos. (unsigned int).

n {
	-Nomes dos campos. (string).
	-Tipos. (TIPO).
	-Booleans. (byte) (se tem default, not null, unique, se é primary key,
			se é foreign key).
	-Tamanhos. (unsigned int) (para strings).
}

-Quant páginas

*/

#ifndef HEADER_H
#define HEADER_H

class Banco;

#include <list>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "util.h"
#include "banco.h"

using namespace std;


#define TAMPAG 32767
#define TAMPAGUSO 32765



__attribute__((__packed__))
struct STRUCT_COLUNA{
	char nome[MAXSTR];
	unsigned char tipo;
	bool notnull;
	bool unique;
	unsigned int tamcampo;
};

__attribute__((__packed__))
struct STRUCT_CABECALHO{
	char nome[MAXSTR];
	unsigned char qcampos;
	unsigned int qpags;
};

typedef struct STRUCT_COLUNA coluna;
typedef struct STRUCT_CABECALHO cabecalho;

class Header {


	public:
		Banco *banco;
		cabecalho cab;
		coluna *cols;
		void *pags;
		int qpags;


		/** Lê um arquivo de header em uma estrutura de header.
		*/
		Header(Banco *b, char *nome);

		Header(Banco *b, cabecalho cab, coluna *cols);

		/** Desaloca a memória alocada em uma header struct.
		*/
		~Header();


		/** Cria um arquivo de los muertos.
		*/
		void criar_arq();


		/** Diz a quantidade total de bytes por registro.
		*/
		size_t get_tam_reg();

		/** Quantidade de registros que cabem em uma pagina.
		*/
		int regs_por_pag();



		void ler_dados();

		void insert(void *reg);

		void *make_reg(...);
};




#endif
