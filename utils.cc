/*
 * utils.c
 *
 *  Created on: 27/11/2011
 */
#include <utils.h>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>

using namespace rokdb;

extern "C" {

UnicodeString ucopy(const UnicodeString &message) {
	UnicodeString new_string;
	char buffer[message.length() * sizeof(UChar)];

	message.extract(0, message.length(), buffer, 255);
	new_string = buffer;
	return new_string;
}

char *cstr(const UnicodeString &message) {
	size_t size = (message.length() + 1) * sizeof(UChar);
	char *buffer = (char *) malloc(size * sizeof(char));

	memset(buffer, 0, size);
	message.extract(0, message.length(), buffer, size);
	return buffer;
}

bool DirectoryExists(char *directory) {
	struct stat sb;

	return stat(directory, &sb) == 0 && S_ISDIR(sb.st_mode);
}

}
