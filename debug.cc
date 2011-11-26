/*
 * debug.c
 *
 *  Created on: 25/11/2011
 */
#include <iostream>
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

}
