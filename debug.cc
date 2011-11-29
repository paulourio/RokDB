/*
 * debug.c
 *
 *  Created on: 25/11/2011
 */
#include <iostream>
#include <unicode/unistr.h>
#include <unicode/regex.h>
#include <cstring>
#include <pthread.h>

#include <debug.h>
#include <config.h>
#include <rokdb.h>

using namespace rokdb;

extern RokDB core;

extern "C" {

void debug(const int level, const std::string message) {
	if (level < core.get_config().get_debug()) {
		switch (level) {
		case 0:
			break;
		case 1:
			std::clog << "\033[32;1m";
			break;
		case 2:
			std::clog << "\033[35m";
			break;
		case 3:
			std::clog << "\033[1m";
			break;
		case 4:
			std::clog << "\033[36m";
			break;
		}
		std::stringstream msg;

		msg << "(" << pthread_self() << "): ";
		msg << message;

		std::clog << msg.str() << std::endl;
		std::clog << "\033[0m";
		std::clog.flush();
	}
}

void error(const std::string message) {
	std::cerr << "\033[31;1m";
	std::cerr << message << std::endl;
	std::cerr << "\033[0m";
	std::cerr.flush();
}

void uprint(const UnicodeString &message) {
	char buffer[255];

	memset(buffer, 0, 255);
	message.extract(0, message.length() + 1, buffer, 255);
	std::cerr << buffer;
	std::cerr.flush();
}

}
