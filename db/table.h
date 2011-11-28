/*
 * table.h
 *
 *  Created on: 27/11/2011
 */
#ifndef TABLE_H_
#define TABLE_H_

#include <unicode/unistr.h>
#include <list>

#include "column.h"

namespace rokdb {

typedef std::list<UnicodeString> Record;
typedef std::list<Record> RecordList;

class Table {
private:
	void FreeColumns();
	void FreeRecords();
public:
	UnicodeString name;
	ColumnList columns;
	RecordList records;
	~Table();
	Column *NewColumn();
};

}

#endif /* TABLE_H_ */
