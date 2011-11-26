/*
 * debug.cc
 *
 *  Created on: 26/11/2011
 */
/*
 * debug.c
 *
 *  Created on: 25/11/2011
 *  Alternative debug implementation for ../debug.cc
 *  This implementation does not use RokDB instance to
 *  access any configuration.
 */
#include <iostream>
#include <debug.h>

using namespace rokdb;

extern "C" {

void debug(const int level, const std::string message) {
	std::clog << message << std::endl;
	std::clog.flush();
}

void error(const std::string message) {
	std::cerr << message << std::endl;
	std::cerr.flush();
}

}
