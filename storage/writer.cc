/*
 * writer.c
 *
 *  Created on: 27/11/2011
 */
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <debug.h>
#include <utils.h>
#include "writer.h"
#include <db/column.h>

using namespace rokdb;

StorageWriter::StorageWriter(UnicodeString &database) : Storage(database) {

}

bool StorageWriter::LoadTable(const Table &table) {
	ClearData();
	ColumnList::iterator it;
	ColumnList list = table.columns;

	table.name.extract(0, table.name.length(), this->header.name, MAX_NAMESTR);
	this->header.columns_count = table.columns.size();
	for (it = list.begin(); it != list.end(); it++) {
		union StorageTableColumn *column;
		column = (union StorageTableColumn *) malloc(sizeof(union StorageTableColumn));
		if (column == NULL) {
			error("No memory to load table data.");
			return false;
		}
		memset(column, 0, sizeof(union StorageTableColumn));
		it->name.extract(0, it->name.length(), column->name, MAX_NAMESTR);
		column->not_null = it->not_null? 1: 0;
		column->unique = it->unique? 1: 0;
		column->size = it->size;
		column->type = 0;
		this->columns.push_back(column);
	}
	return true;
}

bool StorageWriter::Write() {
	if (!DirectoryExists(this->directory)) {
		error("Database does not exists.");
		return false;
	}
	char filename[MAX_STRING];

	BuildTablePath(filename, MAX_STRING, this->directory, this->header.name);

	return WriteHeader(filename) && WriteData(filename);
}

bool StorageWriter::WriteHeader(char *table) {
	char filename[MAX_STRING];
	int written;

	memset(filename, 0, MAX_STRING);
	strcpy(filename, table);
	strcat(filename, HEADER_EXTENSION);
	file = fopen(filename, "w");
	if (file == NULL) {
		error("Can't open table to write.");
		return false;
	}
	/* Write start of header */
	written = fwrite(&this->header, sizeof(this->header), 1, file);
	if (written != 1) {
		error("Can't write table");
		fclose(file);
		return false;
	}
	/* Write columns */
	StorageTableColumnList::iterator it;
	for (it = this->columns.begin(); it != this->columns.end(); it++) {
		written = fwrite(*it, sizeof(**it), 1, file);
		if (written != 1) {
			error("Can't write table");
			fclose(file);
			return false;
		}
	}
	fflush(file);
	fclose(file);
	return true;
}

bool StorageWriter::WriteData(char *table) {
/*	char filename[MAX_STRING];


	strcpy(filename, table);
	strcat(filename, ".data");
	file = fopen(filename, "w");
	if (file == NULL) {
		error("Can't open table to write.");
		return false;
	}
	int written = fwrite(&header, sizeof(header), 1, file);
	if (written != 1) {
		error("Can't write table");
		fclose(file);
		return false;
	}
	fflush(file);
	fclose(file);*/
	return true;
}
