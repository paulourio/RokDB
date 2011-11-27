/*
 * utils.c
 *
 *  Created on: 27/11/2011
 */
#include <utils.h>

using namespace rokdb;

extern "C" {

UnicodeString ucopy(const UnicodeString &message) {
	UnicodeString new_string;
	char buffer[message.length() * sizeof(UChar)];

	message.extract(0, message.length(), buffer, 255);
	new_string = buffer;
	return new_string;
}

}
