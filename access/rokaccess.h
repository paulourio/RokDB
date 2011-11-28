/*
 * rokaccess.h
 *
 *  Created on: 27/11/2011
 */

#ifndef ROKACCESS_H_
#define ROKACCESS_H_

#include <protocol/protocolv1.h>

namespace rokdb {

/* Make access to the database */
class RokAccess {
public:
	static void HandleInsert(const struct CommandInsert *);
	static void HandleNewDatabase(const struct CommandDatabase *);
	static void HandleDestroyDatabase(const struct CommandDatabase *);
	static void HandleCreateTable(const struct CommandCreate *);
};

}

#endif /* ROKACCESS_H_ */
