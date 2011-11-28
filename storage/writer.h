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
	bool RemoveHeader(char *);
	bool RemoveData(char *);
	void ShowErrno();
public:
	StorageWriter(UnicodeString &);
	bool LoadTable(const Table &); /* Load table data from memory */
	bool Write();
	bool RemoveTable(const UnicodeString &); /* Delete table's files from fs */
};

}

#endif /* WRITER_H_ */
