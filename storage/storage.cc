/*
 * storage.cc
 *
 *  Created on: 28/11/2011
 */
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unicode/unistr.h>

#include "storage.h"

using namespace rokdb;

Storage::Storage(UnicodeString &database) : file(NULL) {
	memset(directory, 0, MAX_NAMESTR);
	database.extract(0, database.length(), directory, MAX_NAMESTR);
}
Storage::~Storage() {
	FreeColumns();
}

void Storage::ClearData() {
	FreeColumns();
	memset(&header, 0, sizeof(header));
}

void Storage::FreeColumns() {
	while (!columns.empty()) {
		free(columns.front());
		columns.pop_front();
	}
}
