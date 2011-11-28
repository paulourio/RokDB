/*
 * database.cc
 *
 *  Created on: 27/11/2011
 */
#include <cstring>
#include <utils.h>
#include <sys/stat.h>
#include "database.h"

using namespace rokdb;

Database::Database(const UnicodeString &name) {
	database_name = ucopy(name);
}

Database::~Database() {
}

bool Database::Create() {
	char *directory = cstr(database_name);
	bool result = false;

	if (!Exists() && mkdir(directory, 0222) == 0)
		result = true;
	free(directory);
	return result;
}

bool Database::Destroy() {
	char *directory = cstr(database_name);
	char cmd[200];

	memset(cmd, 0, 200);
	strcpy(cmd, "rm -Rf ");
	strcat(cmd, directory);
	free(directory);
	return system(cmd) == 0;
}

bool Database::Exists() {
	char *directory = cstr(database_name);
	struct stat st;
	bool exists = stat(directory, &st) == 0;

	free(directory);
	return exists;
}
