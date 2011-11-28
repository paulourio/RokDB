/*
 * rokaccess.cc
 *
 *  Created on: 27/11/2011
 */
#include <iostream>
#include <cstring>

#include "rokaccess.h"
#include <rokdb.h>
#include <db/database.h>
#include <utils.h>

using namespace rokdb;

extern RokDB core;

void RokAccess::HandleInsert(const struct CommandInsert *info) {
	std::cout << "Recebi comando de inserção!" << std::endl;
	std::cout << "Banco: ";
	uprint(info->database);

	std::cout << " - Tabela: ";
	uprint(info->table_name);
	std::cout << std::endl;


	StringPair::iterator it;
	StringPair columns = info->columns;

	std::cout << "Colunas: " << info->columns.size() << std::endl;

	for (it = columns.begin(); it != columns.end(); it++) {
		char valor[255], campo[255];

		memset(valor, 0, 255);
		memset(campo, 0, 255);
		it->first.extract(0, 255, campo, 255);
		it->second.extract(0, 255, valor, 255);
		std::cout << "Campo " << campo << " com valor " << valor << std::endl;
	}
}

void RokAccess::HandleNewDatabase(const struct CommandDatabase *info) {
	core.AcquireLock();
	Database db(info->database_name);
	core.lastResult = db.Create();
}

void RokAccess::HandleDestroyDatabase(const struct CommandDatabase *info) {
	core.AcquireLock();
	Database db(info->database_name);
	core.lastResult = db.Exists() && db.Destroy() && !db.Exists();
}
