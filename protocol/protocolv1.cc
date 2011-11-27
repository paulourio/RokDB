/*
 * protocolv1.cc
 *
 *  Created on: 26/11/2011
 */

#include "protocolv1.h"
#include <unicode/regex.h>
#include <rokdb.h>

using namespace rokdb;

extern RokDB core;

ProtocolV1::ProtocolV1() :
	insert_callback(NULL), delete_callback(NULL), update_callback(NULL),
			create_callback(NULL), use_callback(NULL) {
	UnicodeString insert_regex("^\\s*in\\s+(\\S+)\\s+insert\\s+(.+)\\s*;\\s*$");

	RegisterTrigger(insert_regex, (ProtocolTrigger) &ProtocolV1::CommandInsert);
}

ProtocolV1::~ProtocolV1() {

}

#include <iostream>
#include <cstring>
#include <debug.h>
void ProtocolV1::CommandInsert(RegexMatcher *matcher) {
	const UnicodeString insert_fields_regex("(\\S+)=\"(\\S+)\"[,;]?");
	struct CommandInsert info;
	UErrorCode status(U_ZERO_ERROR);

	if (core.get_parser().insert_callback == NULL)
		return;
	info.table_name = matcher->group(1, status);

	UnicodeString param(matcher->group(2, status));
	RegexMatcher *fields = Match(insert_fields_regex, param);
	if (fields != NULL) {
		while (fields->find()) {
			UnicodeString field = fields->group(1, status);
			UnicodeString value = fields->group(2, status);

			info.columns.insert(std::make_pair(field, value));
		}
		delete fields;
	}
	core.get_parser().insert_callback(&info);
}

void ProtocolV1::OnInsert(ProtocolEventInsert callback) {
	insert_callback = callback;
}

void ProtocolV1::OnDelete(ProtocolEventDelete callback) {
	delete_callback = callback;
}

void ProtocolV1::OnUpdate(ProtocolEventUpdate callback) {
	update_callback = callback;
}

void ProtocolV1::OnCreate(ProtocolEventCreate callback) {
	create_callback = callback;
}

void ProtocolV1::OnUse(ProtocolEventUse callback) {
	use_callback = callback;
}

void ProtocolV1::OnCommit(ProtocolEventCommit callback) {
	commit_callback = callback;
}
