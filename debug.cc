/*
 * debug.c
 *
 *  Created on: 25/11/2011
 */
#include <iostream>
#include <unicode/unistr.h>
#include <unicode/regex.h>
#include <cstring>

#include <debug.h>
#include <config.h>
#include <rokdb.h>

using namespace rokdb;

extern RokDB core;

extern "C" {

void debug(const int level, const std::string message) {
	if (level < core.get_config().get_debug()) {
		std::clog << message << std::endl;
		std::clog.flush();
	}
}

void error(const std::string message) {
	std::cerr << message << std::endl;
	std::cerr.flush();
}

void uprint(const UnicodeString &message) {
	UErrorCode status(U_ZERO_ERROR);
	char buffer[255];

	memset(buffer, 0, 255);
	message.extract(0, message.length() +1 , buffer, 255);
	std::cerr << buffer;
	std::cerr.flush();
}

}
