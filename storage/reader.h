/*
 * reader.h
 *
 *  Created on: 27/11/2011
 */

#ifndef READER_H_
#define READER_H_

#include <cstdio>
#include <unicode/unistr.h>

#include "data.h"
#include "storage.h"
#include <db/table.h>
#include <db/column.h>

namespace rokdb {

class StorageReader: private Storage {
private:
	bool ReadHeader(char *);
	bool ReadData(char *);
public:
	StorageReader(UnicodeString &);
	bool Read(Table &);
};

}

#endif /* READER_H_ */
