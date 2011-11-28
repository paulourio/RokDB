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

namespace rokdb {

class Database {
private:
	UnicodeString database_name;
public:
	Database(const UnicodeString &);
	~Database();
	bool Create(); /* Create directory structure */
	bool Destroy(); /* Remove directory structure */
	bool Exists();
};

}

#endif /* DATABASE_H_ */
