/*
 * client.h
 *
 *  Created on: 25/11/2011
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <unicode/unistr.h>
#include <socket.h>

namespace rokdb {

class ClientSocket: private Socket {
public:
	ClientSocket(std::string host, int port);
	virtual ~ClientSocket() {
	}
	;

	const ClientSocket& operator <<(const UnicodeString&) const;
	const ClientSocket& operator >>(UnicodeString&) const;
};

}

#endif /* CLIENT_H_ */
