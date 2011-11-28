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

}

Column *Table::NewColumn() {
	Column *col = (Column *) malloc(sizeof(Column));
	if (col != NULL)
		memset(col, 0, sizeof(Column));
	return col;
}
