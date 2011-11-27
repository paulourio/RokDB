#include "util.h"


void arqext(char *final, char *arq, char *ext) {
	strcpy(final, arq);
	strcat(final, ".");
	strcat(final, ext);
}



void pastaarq(char *final, char *pasta, char *arq, char *ext) {
	strcpy(final, pasta);
	strcat(final, "/");
	char guano[MAXSTR];
	arqext(guano, arq, ext);
	strcat(final, guano);
}
