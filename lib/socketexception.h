/*
 * socketexception.hpp
 *
 *  Created on: 23/11/2011
 */

#ifndef SOCKETEXCEPTION_HPP_
#define SOCKETEXCEPTION_HPP_

#include <string>

namespace rokdb {

class SocketException {
private:
	std::string message_str;
public:
	const int error;

	SocketException(std::string);
	std::string description() const;
};

}

#endif /* SOCKETEXCEPTION_HPP_ */
