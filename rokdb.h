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
#include <protocol/protocolv1.h>

namespace rokdb {

class RokDB {
private:
	const std::string config_file;
	Config config;
	RokServer server;
	AppMutex mutex;
	ProtocolV1 parser;
	pthread_mutex_t lock;

	static void SignalHandler(int);
	void Lock();
public:
	RokDB();
	~RokDB();
	void start(void);

	RokServer get_server();
	Config get_config();
	ProtocolV1 get_parser();

	void AcquireLock();
	void FreeLock();

	bool lastResult; /* Thread get here the result of last executed command */
	 /* Out buffer. First thread that find this buffer not empty will send
	  * to client. */
	UnicodeString responseBuffer;
	bool shouldClose; /* Should close connection after response. */
	bool rawData; /* Set false and it will not send last result. */
};

}

#endif /* ROKDB_H_ */
