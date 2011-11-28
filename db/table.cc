/*
 * table.cc
 *
 *  Created on: 28/11/2011
 */
#include <cstdlib>
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
