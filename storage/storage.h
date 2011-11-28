/*
 * storage.h
 *
 *  Created on: 28/11/2011
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <list>
#include <cstdio>
#include <unicode/unistr.h>

#include <db/table.h>
#include "data.h"

namespace rokdb {

typedef std::list<union StorageTableColumn *> StorageTableColumnList;

class Storage {
public:
	FILE *file;
	char directory[MAX_STRING];
	struct StorageTableHeader header;
	StorageTableColumnList columns;

	void ClearData(); /* Initialize header and clear column list */

	Storage(UnicodeString &);
	~Storage();
private:
	void FreeColumns();
};

}

#endif /* STORAGE_H_ */
