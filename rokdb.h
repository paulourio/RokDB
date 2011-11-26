/*
 * rokdb.h
 *
 *  Created on: 25/11/2011
 */

#ifndef ROKDB_H_
#define ROKDB_H_

#include <string>
#include <rokserver.h>
#include <config.h>
#include "appmutex.h"

namespace rokdb {

class RokDB {
private:
	const std::string config_file;
	Config config;
	RokServer server;
	AppMutex mutex;
	static void SignalHandler(int);
	void Lock();
public:
	RokDB();
	~RokDB();
	void start(void);

	RokServer get_server();
	Config get_config();
};

}

#endif /* ROKDB_H_ */
