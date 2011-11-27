/*
 * rokaccess.cc
 *
 *  Created on: 27/11/2011
 */
#include <iostream>
#include <cstring>
#include "rokaccess.h"
#include <rokdb.h>

using namespace rokdb;

extern RokDB core;

void RokAccess::HandleInsert(const struct CommandInsert *info) {
	char tabela[255];

	memset(tabela, 0, 255);
	info->table_name.extract(0, 255, tabela, 255);
	std::cout << "Recebi comando de inserção!" << std::endl;
	std::cout << "Tabela: " << tabela << std::endl;
	StringPair::iterator it;
	StringPair columns = info->columns;

	std::cout << "Colunas: " << info->columns.size() << std::endl;

	for (it = columns.begin(); it != columns.end(); it++) {
		char valor[255];

		memset(valor, 0, 255);
		it->first.extract(0, 255, tabela, 255);
		it->second.extract(0, 255, valor, 255);
		std::cout << "Campo " << tabela << " com valor " << valor << std::endl;
	}
}
