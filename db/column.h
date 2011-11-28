/*
 * column.h
 *
 *  Created on: 27/11/2011
 */

#ifndef COLUMN_H_
#define COLUMN_H_

namespace rokdb {

struct Column {
	UnicodeString name;
	bool not_null;
	bool unique;
	int size;
};

typedef struct Column Column;
typedef std::list<Column *> ColumnList;

}

#endif /* COLUMN_H_ */
