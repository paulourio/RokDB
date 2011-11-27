/*
 * debug.hpp
 *
 *  Created on: 24/11/2011
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <unicode/unistr.h>
#include <string>

namespace rokdb {

extern "C" {

void debug(const int, const std::string);
void error(const std::string);
void uprint(const UnicodeString &message);

}

}

#endif /* DEBUG_HPP_ */
