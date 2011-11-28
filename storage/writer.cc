/*
 * writer.c
 *
 *  Created on: 27/11/2011
 */
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <map>
#include <unicode/unistr.h>

#include <debug.h>
#include <utils.h>
#include "writer.h"
#include "data.h"
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
		Column *col = *it;
		memset(column, 0, sizeof(union StorageTableColumn));
		col->name.extract(0, col->name.length(), column->name, MAX_NAMESTR);
		column->not_null = col->not_null? 1: 0;
		column->unique = col->unique? 1: 0;
		column->type = col->type;
		this->columns.push_back(column);
	}
	/* Records */
	records.assign(table.records.begin(), table.records.end());
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

	memset(filename, 0, sizeof(filename));
	strcpy(filename, table);
	strcat(filename, HEADER_EXTENSION);
	file = fopen(filename, "wb");
	if (file == NULL) {
		error("Can't open table header to write.");
		ShowErrno();
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
	char filename[MAX_STRING];

	memset(filename, 0, sizeof(filename));
	strcpy(filename, table);
	strcat(filename, DATA_EXTENSION);
	file = fopen(filename, "wb");
	if (file == NULL) {
		error("Can't open table data to write.");
		ShowErrno();
		return false;
	}
	ColumnValues::iterator cols;
	RecordList::iterator it;
	char buffer[MAX_STRING];
	for (it = records.begin(); it != records.end(); it++) {
		ColumnValues *itcol = *it;
		for (cols = itcol->begin(); cols != itcol->end(); cols++) {
			/* Write its value */
			memset(buffer, 0, sizeof(buffer));
			cols->extract(0, cols->length(), buffer, sizeof(buffer));
			if (fwrite(buffer, sizeof(buffer), 1, file) != 1) {
				error("Can't write table data.");
				fclose(file);
				return false;
			}
		}
	}
	fflush(file);
	fclose(file);
	return true;
}

bool StorageWriter::RemoveTable(const UnicodeString &table) {
	char filename[MAX_STRING];
	char *ctable = cstr(table);

	BuildTablePath(filename, sizeof(filename), this->directory, ctable);
	free(ctable);
	return RemoveHeader(filename) && RemoveData(filename);
}

bool StorageWriter::RemoveHeader(char *table) {
	char filename[MAX_STRING];
	int code;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, table);
	strcat(filename, HEADER_EXTENSION);
	if (!FileExists(filename))
		return false;
	code = std::remove(filename);
	if (code == -1)
		ShowErrno();
	return code == 0;
}

bool StorageWriter::RemoveData(char *table) {
	char filename[MAX_STRING];
	int code;

	memset(filename, 0, MAX_STRING);
	strcpy(filename, table);
	strcat(filename, DATA_EXTENSION);
	/* .data is not a required file for a table (ie empty).
	 * The operation should be successful with this file doesn't exists. */
	if (!FileExists(filename))
		return true;
	code = std::remove(filename);
	if (code == -1)
		ShowErrno();
	return code == 0;
}

void StorageWriter::ShowErrno() {
	extern int errno;

	switch (errno) {
	case EACCES:
		error("StorageWriter: Permission denied for file handling.");
		break;
	case EIO:
		error("StorageWriter: I/O error.");
		break;
	case ENOMEM:
		error("StorageWriter: No memory in kernel for operation.");
		break;
	case EROFS:
		error("StorageWriter: Database in read-only filesystem.");
		break;
	case EINVAL:
		error("StorageWriter: Invalid permissions.");
		break;
	}
}
