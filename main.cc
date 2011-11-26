/*
 * main.cc
 *
 *  Created on: 25/11/2011
 */
#include <cstdlib>
#include "rokdb.h"

using namespace rokdb;

RokDB core;

int main(void) {
	core.start();
	return EXIT_SUCCESS;
}
