/*
 * reader.cc
 *
 *  Created on: 28/11/2011
 */
#include <cstdlib>
#include <cstring>
#include <debug.h>
#include <utils.h>
#include "reader.h"

using namespace rokdb;

StorageReader::StorageReader(UnicodeString &database) : Storage(database) {

}

bool StorageReader::Read(Table &table) {
	if (!DirectoryExists(this->directory)) {
		error("Database does not exists.");
		return false;
	}
	char filename[MAX_STRING];

	ClearData();
	table.name.extract(0, table.name.length(), this->header.name, MAX_NAMESTR);
	BuildTablePath(filename, MAX_STRING, this->directory, this->header.name);
	return ReadHeader(filename, table) && ReadData(filename, table);
}

#include <iostream>
bool StorageReader::ReadHeader(char *path, Table &table) {
	char filename[MAX_STRING];
	int read;

	memset(filename, 0, MAX_STRING);
	strcpy(filename, path);
	strcat(filename, HEADER_EXTENSION);
	file = fopen(filename, "r");
	std::cout << filename << std::endl;
	if (file == NULL) {
		error("Can't open table to read.");
		return false;
	}
	/* Read start of header */
	if (fseek(file, 0, SEEK_SET) == -1 || feof(file)) {
		error("Can't read table.");
		fclose(file);
		return false;
	}
	read = fread(&this->header, sizeof(this->header), 1, file);
	if (read != 1) {
		error("Can't read table");
		fclose(file);
		return false;
	}
	/* Read columns */
	int i=0;
	while (!feof(file)) {
		StorageTableColumn *column;

		column = (StorageTableColumn *) malloc(sizeof(StorageTableColumn));
		if (column == NULL) {
			error("No memory to read table columns.");
			fclose(file);
			return false;
		}
		read = fread(column, sizeof(StorageTableColumn), 1, file);
		if (read != 1) {
			free(column);
			if (feof(file))
				break;
			error("Can't read column table");
			fclose(file);
			return false;
		}
		this->columns.push_back(column);
		i++;
	}
	fclose(file);

	/* Copy from storage structures to db */
	table.name = this->header.name;
	StorageTableColumnList::iterator it;
	StorageTableColumnList list = this->columns;
	for (it = list.begin(); it != list.end(); it++) {
		Column *col = (Column *) malloc(sizeof(Column));
		if (col == NULL) {
			error("Can't create column representation while reading table.");
			return false;
		}
		memset(col, 0, sizeof(Column));
		StorageTableColumn *cit = *it;
		col->name = cit->name;
		col->not_null = cit->not_null == 1? true: false;
		col->unique = cit->unique == 1? true: false;
		col->size = cit->size;
		table.columns.push_back(col);
	}
	return true;
}

bool StorageReader::ReadData(char *path, Table &table) {
	return true;
}
