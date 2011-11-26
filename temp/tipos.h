/**

20/set/2011

Controla os tipos de dados que o banco aceita.


todo: tudo (ler, escrever).



*/
#ifndef TIPOS_H
#define TIPOS_H

#include <time.h>
#include "byteio.h"
#include "data.h"


#define MAXSTR 256


typedef enum {

	tint,
	tfloat,
	tstring
}TIPO;

#define is_var(t) (t == varchar_utf8 || t == varchar_utf16 || \
		 t == varchar_utf32)

#define is_fix(t)  (t == fixchar_utf8 || t == fixchar_utf16 || \
		 t == fixchar_utf32)



/** Retorna o tamanho do tipo.
*/
size_t get_tam(TIPO t);



void ler_valor(FILE *f, void *v, TIPO t, int n);

#endif
