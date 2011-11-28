/*
 * database.cc
 *
 *  Created on: 27/11/2011
 */
#include <cstring>
#include <utils.h>
#include <sys/stat.h>

#include "database.h"
#include "table.h"
#include <storage/writer.h>

using namespace rokdb;

Database::Database(const UnicodeString &name) {
	database_name = ucopy(name);
	cdatabase_name = cstr(name);
}

Database::~Database() {
	if (cdatabase_name != NULL)
		free(cdatabase_name);
}

bool Database::Create() {
	return !Exists() && mkdir(cdatabase_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
}

bool Database::Destroy() {
	char cmd[200];

	memset(cmd, 0, 200);
	strcpy(cmd, "rm -Rf ");
	strcat(cmd, cdatabase_name);
	return system(cmd) == 0;
}

bool Database::Exists() {
	struct stat st;
	return stat(cdatabase_name, &st) == 0;
}

#include <iostream>
Table *Database::ReadTable(const UnicodeString &name) {
	char *table_name = cstr(name);
	char path[255];

	BuildTablePath(path, 255, cdatabase_name, table_name);
	if (!FileExists(path)) {
		free(table_name);
		return NULL;
	}



	std::cout << path << std::endl;

	free(table_name);
	return NULL;
}

Table *Database::CreateTable(const UnicodeString &name) {
	char *table_name = cstr(name);
	char path[255];

	BuildTablePath(path, 255, cdatabase_name, table_name);
	if (FileExists(path)) {
		free(table_name);
		return NULL;
	}

	Table *table = new Table();
	table->name = ucopy(name);
	free(table_name);
	return table;
}

bool Database::WriteTable(Table &table) {
	StorageWriter writer(database_name);

	writer.LoadTable(table);
	return writer.Write();
}

bool Database::DropTable(const UnicodeString &name) {
	StorageWriter writer(database_name);

	return writer.RemoveTable(name);
}
