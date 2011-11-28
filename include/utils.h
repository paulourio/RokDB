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

UnicodeString ucopy(const UnicodeString &);

/* Convert an UnicodeString to C string */
char *cstr(const UnicodeString &);

bool DirectoryExists(char *directory);

}

}

#endif /* UTILS_H_ */
