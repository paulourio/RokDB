/*
 * table.h
 *
 *  Created on: 27/11/2011
 */
#ifndef TABLE_H_
#define TABLE_H_

#include <unicode/unistr.h>
#include <list>
#include <map>
#include <utility>

#include "column.h"

namespace rokdb {

typedef std::list<UnicodeString> ColumnValues;
typedef std::list<ColumnValues *> RecordList;

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
	int GetColumnIndex(const UnicodeString &);
};

}

#endif /* TABLE_H_ */
