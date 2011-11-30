/*
 * rokaccess.cc
 *
 *  Created on: 27/11/2011
 */
#include <iostream>
#include <cstring>
#include <map>
#include <utility>

#include "rokaccess.h"
#include <rokdb.h>
#include <db/database.h>
#include <db/table.h>
#include <utils.h>

using namespace rokdb;

extern RokDB core;

void RokAccess::HandleInsert(const struct CommandInsert *info) {
	core.AcquireLock();
	char *tbl = cstr(info->table_name);
	if (tbl != NULL) {
		std::stringstream msg;
		msg << "Inserindo na tabela " << tbl;
		debug(5, msg.str());
		free(tbl);
	}
	core.lastResult = false;
	Database db(info->database);
	Table *table = db.ReadTable(info->table_name);
	if (table == NULL)
		return;

	ColumnValues *vals = new ColumnValues();

	vals->assign(info->values.begin(), info->values.end());
	table->records.push_back(vals);

	core.lastResult = db.WriteTable(*table);
	delete table;
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
		StringPairList::iterator it;
		StringPairList cols = info->columns;

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

void RokAccess::HandleSelect(const struct CommandSelect *info) {
	core.AcquireLock();
	core.lastResult = false;
	Database db(info->database);
	Table *table = db.ReadTable(info->table_name);

	std::stringstream record_list;
	int record_count = 0;

	RecordList::iterator rit;
	for (rit = table->records.begin(); rit != table->records.end(); rit++) {
		ColumnValues::iterator cit;
		ColumnValues *cols = *rit;
		bool valid = true;

		if (!info->all) {
			StringPairList spl = info->conditions;
			StringPairList::iterator spl_it;

			for (spl_it = spl.begin(); spl_it != spl.end(); spl_it++) {
				int index = table->GetColumnIndex(spl_it->first);
				if (index == -1) /* Column not found */
					return;
				UnicodeString first, second;
				first = spl_it->second;
				first.toLower();
				cit = cols->begin();
				while (index--)
					cit++;
				second = ucopy(*cit);
				second.toLower();
				if (first.compare(second) != 0) {
					valid = false;
					break;
				}
			}
		}
		if (!valid)
			continue;

		cols = *rit;
		for (cit = cols->begin(); cit != cols->end(); cit++) {
			char *c = cstr(ucopy(*cit));
			record_list << c;
			record_list << "\t";
			free(c);
		}
		record_count++;
		record_list << "\n";
	}

	std::stringstream count;
	count << record_count << "\n";
	core.responseBuffer = count.str().c_str();
	core.responseBuffer += record_list.str().c_str();

	char *ret = cstr(core.responseBuffer);
	if (ret != NULL) {
		std::stringstream sret;
		sret << ret;
		debug(5, sret.str());
		free(ret);
	}

	core.lastResult = true;
	delete table;
}
