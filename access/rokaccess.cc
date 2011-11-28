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
#include <db/table.h>
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

void RokAccess::HandleCreateTable(const struct CommandCreate *info) {
	core.AcquireLock();
	core.lastResult = false;
	Database db(info->database);
	if (!db.Exists())
		return;

	Table *table = db.CreateTable(info->table_name);
	if (table != NULL) {
		StringPair::iterator it;
		StringPair cols = info->columns;

		for (it = cols.begin(); it != cols.end(); it++) {
			Column *col = table->NewColumn();
			col->name = ucopy(it->first);
			col->not_null = false;
			col->unique = false;
			col->type = ProtocolV1::IdentifyColumnType(it->second);
			table->columns.push_back(col);
		}

		core.lastResult = db.WriteTable(*table);
		delete table;
	}
}

void RokAccess::HandleDropTable(const struct CommandDrop *info) {
	core.AcquireLock();
	core.lastResult = false;
	Database db(info->database);
	if (!db.Exists())
		return;
	core.lastResult = db.DropTable(info->table_name);
}
