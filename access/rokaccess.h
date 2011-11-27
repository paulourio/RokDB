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
	static void HandleInsert(const struct CommandInsert *info);
	static void HandleNewDatabase(const struct CommandDatabase *info);
};

}

#endif /* ROKACCESS_H_ */
