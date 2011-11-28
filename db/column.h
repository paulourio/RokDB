/*
 * column.h
 *
 *  Created on: 27/11/2011
 */

#ifndef COLUMN_H_
#define COLUMN_H_

namespace rokrb {

class Column {
public:
	UnicodeString name;
	bool not_null;
	bool unique;
	int size;
};

typedef std::list<Column> ColumnList;

}

#endif /* COLUMN_H_ */
