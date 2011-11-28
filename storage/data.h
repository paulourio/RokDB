/*
 * data.h
 *
 *  Created on: 27/11/2011
 */

#ifndef DATA_H_
#define DATA_H_

///#include <cinttypes>

namespace rokdb {

extern "C" {

#define MAX_NAMESTR 255
#define MAX_STRING 1024

#define HEADER_EXTENSION ".header"
#define DATA_EXTENSION ".data"

union StorageTableColumn {
	uint32_t column[33];
	struct {
		char name[MAX_NAMESTR];
		unsigned char type: 2;
		int not_null: 1;
		int unique: 1;
		uint16_t size;
	};
} __attribute__((__packed__));

struct StorageTableHeader {
	char name[MAX_NAMESTR];
	unsigned char columns_count;
} __attribute__((__packed__));

}

}

#endif /* DATA_H_ */
