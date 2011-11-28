/*
 * protocolv1.h
 *
 *  Created on: 26/11/2011
 */

#ifndef PROTOCOLV1_H_
#define PROTOCOLV1_H_

#include <map>
#include <utility>
#include <unicode/unistr.h>
#include "protocol.h"

namespace rokdb {

typedef std::pair<UnicodeString, UnicodeString> StringPair;
typedef std::list<StringPair> StringPairList;
typedef std::list<UnicodeString> StringList;

struct CommandCreate {
	UnicodeString database;
	UnicodeString table_name;
	StringPairList columns; /* <column name, type> */
};

struct CommandDrop {
	UnicodeString database;
	UnicodeString table_name;
};

struct CommandInsert {
	UnicodeString database;
	UnicodeString table_name;
	StringList values; /* <value> */
};

struct CommandDelete {
	UnicodeString database;
	UnicodeString table_name;
	StringPairList conditions; /* <column, value> */
};

struct CommandSelect {
	UnicodeString database;
	UnicodeString table_name;
	StringPairList conditions; /* <column, value> */
	bool all;
};

struct CommandUpdate {
	UnicodeString database;
	UnicodeString table_name;
	StringPairList conditions; /* <column, value> */
	StringPairList new_values; /* <column, value> */
};

struct CommandDatabase {
	UnicodeString database_name;
};

typedef void (*ProtocolEventInsert)(const struct CommandInsert *);
typedef void (*ProtocolEventDelete)(const struct CommandDelete *);
typedef void (*ProtocolEventUpdate)(const struct CommandUpdate *);
typedef void (*ProtocolEventCreate)(const struct CommandCreate *);
typedef void (*ProtocolEventDrop)(const struct CommandDrop *);
typedef void (*ProtocolEventDatabase)(const struct CommandDatabase *);
typedef void (*ProtocolEventSelect)(const struct CommandSelect *);

class ProtocolV1: public Protocol {
public:
	ProtocolV1();

	void OnInsert(ProtocolEventInsert);
	void OnDelete(ProtocolEventDelete);
	void OnUpdate(ProtocolEventUpdate);
	void OnCreate(ProtocolEventCreate);
	void OnDrop(ProtocolEventDrop);
	void OnNewDatabase(ProtocolEventDatabase);
	void OnDestroyDatabase(ProtocolEventDatabase);
	void OnSelect(ProtocolEventSelect);

	/* Threaded asynchronous calls */
	static bool CommandInsert(RegexMatcher *);
	static bool CommandNewDatabase(RegexMatcher *);
	static bool CommandDestroyDatabase(RegexMatcher *);
	static bool CommandCreateTable(RegexMatcher *);
	static bool CommandDropTable(RegexMatcher *);
	static bool CommandSelect(RegexMatcher *);

	/* looool */
	ProtocolEventInsert insert_callback;
	ProtocolEventDelete delete_callback;
	ProtocolEventUpdate update_callback;
	ProtocolEventCreate create_callback;
	ProtocolEventDrop drop_callback;
	ProtocolEventDatabase newdatabase_callback;
	ProtocolEventDatabase destroydatabase_callback;
	ProtocolEventSelect select_callback;

	/* const */
	static int IdentifyColumnType(const UnicodeString &);
};

}

#endif /* PROTOCOLV1_H_ */
