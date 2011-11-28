/*
 * table.cc
 *
 *  Created on: 28/11/2011
 */
#include <cstdlib>
#include <cstring>
#include "table.h"

using namespace rokdb;

Table::~Table() {
	FreeColumns();
	FreeRecords();
}


void Table::FreeColumns() {
	while (!columns.empty()) {
		free(columns.front());
		columns.pop_front();
	}
}

void Table::FreeRecords() {
	while (!records.empty()) {
		delete records.front();
		records.pop_front();
	}
}

Column *Table::NewColumn() {
	Column *col = (Column *) malloc(sizeof(Column));
	if (col != NULL)
		memset(col, 0, sizeof(Column));
	return col;
}

int Table::GetColumnIndex(const UnicodeString &column_name) {
	ColumnList::iterator it;
	int pos = 0;

	for (it = columns.begin(); it != columns.end(); it++, pos++)
		if ((*it)->name.compare(column_name) == 0)
			return pos;
	return -1;
}
