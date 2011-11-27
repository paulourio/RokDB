/*
 * protocolv1.h
 *
 *  Created on: 26/11/2011
 */

#ifndef PROTOCOLV1_H_
#define PROTOCOLV1_H_

#include <map>
#include <unicode/unistr.h>
#include "protocol.h"

namespace rokdb {

typedef std::map<UnicodeString, UnicodeString>  StringPair;

struct CommandCreate {
	UnicodeString database;
	UnicodeString table_name;
	StringPair columns; /* <column name, type> */
};

struct CommandInsert {
	UnicodeString database;
	UnicodeString table_name;
	StringPair columns; /* <column name, value> */
};

struct CommandDelete {
	UnicodeString database;
	UnicodeString table_name;
	StringPair conditions; /* <column, value> */
};

struct CommandUpdate {
	UnicodeString database;
	UnicodeString table_name;
	StringPair conditions; /* <column, value> */
	StringPair new_values; /* <column, value> */
};

struct CommandDatabase {
	UnicodeString database_name;
};

typedef void (*ProtocolEventInsert)(const struct CommandInsert *);
typedef void (*ProtocolEventDelete)(const struct CommandDelete *);
typedef void (*ProtocolEventUpdate)(const struct CommandUpdate *);
typedef void (*ProtocolEventCreate)(const struct CommandCreate *);
typedef void (*ProtocolEventDatabase)(const struct CommandDatabase *);

class ProtocolV1: public Protocol {
public:
	ProtocolV1();

	void OnInsert(ProtocolEventInsert);
	void OnDelete(ProtocolEventDelete);
	void OnUpdate(ProtocolEventUpdate);
	void OnCreate(ProtocolEventCreate);
	void OnNewDatabase(ProtocolEventDatabase);
	void OnDestroyDatabase(ProtocolEventDatabase);

	/* Threaded asynchronous calls */
	static void CommandInsert(RegexMatcher *);
	static void CommandNewDatabase(RegexMatcher *);
	static void CommandDestroyDatabase(RegexMatcher *);


	/* looool */
	ProtocolEventInsert insert_callback;
	ProtocolEventDelete delete_callback;
	ProtocolEventUpdate update_callback;
	ProtocolEventCreate create_callback;
	ProtocolEventDatabase newdatabase_callback;
	ProtocolEventDatabase destroydatabase_callback;
};

}

#endif /* PROTOCOLV1_H_ */
