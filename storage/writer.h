/*
 * writer.h
 *
 *  Created on: 27/11/2011
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <cstdio>

#include "storage.h"

namespace rokdb {

class StorageWriter: private Storage {
private:
	bool WriteHeader(char *);
	bool WriteData(char *);
public:
	StorageWriter(UnicodeString &);
	bool LoadTable(const Table &);
	bool Write();
};

}

#endif /* WRITER_H_ */
