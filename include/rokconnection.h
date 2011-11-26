/*
 * rokconnection.hpp
 *
 *  Created on: 24/11/2011
 */

#ifndef ROKCONNECTION_HPP_
#define ROKCONNECTION_HPP_

#include "serversocket.h"

namespace rokdb {

/**
 * Handle a client connection.
 */
class RokConnection {
private:
	bool active;
	ServerSocket *socket;
public:
	RokConnection(ServerSocket *);
	~RokConnection();
	void Listen();
	void RequestStop();
};

}

#endif /* ROKCONNECTION_HPP_ */
