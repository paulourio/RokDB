/*
 * utils.h
 *
 *  Created on: 27/11/2011
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <unicode/unistr.h>

namespace rokdb {

extern "C" {

UnicodeString ucopy(const UnicodeString &message);

}

}

#endif /* UTILS_H_ */
