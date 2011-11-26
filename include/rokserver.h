/*
 * rokserver.hpp
 *
 *  Created on: 24/11/2011
 */

#ifndef ROKSERVER_HPP_
#define ROKSERVER_HPP_

#include <list>
#include <pthread.h>
#include "rokconnection.h"

namespace rokdb {

struct ConnectionInfo {
	RokConnection *connection;
	pthread_t *thread;
};

class RokServer {
private:
	bool active ;
	pthread_t server_thread;
	std::list<struct ConnectionInfo *> connections;

	void CloseConnections();
	static void KillConnection(pthread_t *);

	static void *RunServer(void *);
	static void *NewConnection(void *);
public:
	void Start();
	void Stop();
	void Execute();
	RokServer();
	~RokServer();
	void set_active(bool);
	std::list<struct ConnectionInfo *> get_connections();
	void FreeInfo(struct ConnectionInfo *info);
};

}

#endif /* ROKSERVER_HPP_ */
