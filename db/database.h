/*
 * database.h
 *
 *  Created on: 27/11/2011
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <unicode/unistr.h>
#include <pthread.h>
#include <cstdlib>

#include "table.h"

namespace rokdb {

class Database {
private:
	UnicodeString database_name;
	char *cdatabase_name; /* Copy in C string */
public:
	Database(const UnicodeString &);
	~Database();
	bool Create(); /* Create directory structure */
	bool Destroy(); /* Remove directory structure */
	bool Exists(); /* Is the database created? */
	Table *OpenTable(const UnicodeString &);
};

}

#endif /* DATABASE_H_ */
