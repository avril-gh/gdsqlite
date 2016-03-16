/* gdsqlite.cpp */

#include "gdsqlite.h"
#include "globals.h"
#include "os/os.h"

SQLite::SQLite() {
}

int SQLite::open(String path) {
	if (path.begins_with("res://")) {
		if (Globals::get_singleton()) {
			String resource_path = Globals::get_singleton()->get_resource_path();
			if (resource_path != "") path = path.replace("res:/",resource_path);
			else path = path.replace("res://", "");
		}
	}
	else if (path.begins_with("user://")) {
		String data_dir=OS::get_singleton()->get_data_dir();
		if (data_dir != "") path = path.replace("user:/",data_dir);
		else path = path.replace("user://", "");
	}
	
	return sqlite3_open(path.utf8().get_data(), &db);
}

void SQLite::prepare(String query) {
	sqlite3_prepare_v2(db, query.utf8().get_data(), -1, &stmt, NULL);
}

int SQLite::step() {
	return sqlite3_step(stmt);
}

int SQLite::step_assoc() {
	int ret = sqlite3_step(stmt);
	if (_row_names.size() == 0) {
		for(unsigned int i = 0; i < sqlite3_column_count(stmt); ++i)
			_row_names[sqlite3_column_name(stmt,i)] = i;
	}
	return ret;
}

int SQLite::get_data_count() {
	return sqlite3_data_count(stmt);
}

int SQLite::get_column_count() {
	return sqlite3_column_count(stmt);
}

int SQLite::get_column_int(int col) {
	return sqlite3_column_int(stmt, col);
}

double SQLite::get_column_double(int col) {
	return sqlite3_column_double(stmt, col);
}

String SQLite::get_column_text(int col) {
	return String::utf8((char *)sqlite3_column_text(stmt, col));
}

int SQLite::get_column_int_assoc(String col) {
	return sqlite3_column_int(stmt, _row_names[col]);
}

double SQLite::get_column_double_assoc(String col) {
	return sqlite3_column_double(stmt, _row_names[col]);
}

String SQLite::get_column_text_assoc(String col) {
	return String::utf8((char *)sqlite3_column_text(stmt, _row_names[col]));
}

void SQLite::finalize() {
	sqlite3_finalize(stmt);
	_row_names.clear();
}

String SQLite::get_errormsg() {
	return sqlite3_errmsg(db);
}

void SQLite::close() {
	sqlite3_close(db);
}

void SQLite::_bind_methods() {
	ObjectTypeDB::bind_method("open", &SQLite::open);
	ObjectTypeDB::bind_method("prepare", &SQLite::prepare);
	ObjectTypeDB::bind_method("step", &SQLite::step);
	ObjectTypeDB::bind_method("step_assoc", &SQLite::step_assoc);

	ObjectTypeDB::bind_method("get_data_count", &SQLite::get_data_count);
	ObjectTypeDB::bind_method("get_column_count", &SQLite::get_column_count);
	ObjectTypeDB::bind_method("get_column_int", &SQLite::get_column_int);
	ObjectTypeDB::bind_method("get_column_double", &SQLite::get_column_double);
	ObjectTypeDB::bind_method("get_column_text", &SQLite::get_column_text);
	ObjectTypeDB::bind_method("finalize", &SQLite::finalize);
	ObjectTypeDB::bind_method("get_errormsg", &SQLite::get_errormsg);
	ObjectTypeDB::bind_method("close", &SQLite::close);

	ObjectTypeDB::bind_method("get_column_int_assoc", &SQLite::get_column_int_assoc);
	ObjectTypeDB::bind_method("get_column_double_assoc", &SQLite::get_column_double_assoc);
	ObjectTypeDB::bind_method("get_column_text_assoc", &SQLite::get_column_text_assoc);
	
	BIND_CONSTANT(SQLITE_OK);
	BIND_CONSTANT(SQLITE_ERROR);
	BIND_CONSTANT(SQLITE_INTERNAL);
	BIND_CONSTANT(SQLITE_PERM);
	BIND_CONSTANT(SQLITE_ABORT);
	BIND_CONSTANT(SQLITE_BUSY);
	BIND_CONSTANT(SQLITE_LOCKED);
	BIND_CONSTANT(SQLITE_NOMEM);
	BIND_CONSTANT(SQLITE_READONLY);
	BIND_CONSTANT(SQLITE_INTERRUPT);
	BIND_CONSTANT(SQLITE_IOERR);
	BIND_CONSTANT(SQLITE_CORRUPT);
	BIND_CONSTANT(SQLITE_NOTFOUND);
	BIND_CONSTANT(SQLITE_FULL);
	BIND_CONSTANT(SQLITE_CANTOPEN);
	BIND_CONSTANT(SQLITE_PROTOCOL);
	BIND_CONSTANT(SQLITE_EMPTY);
	BIND_CONSTANT(SQLITE_SCHEMA);
	BIND_CONSTANT(SQLITE_TOOBIG);
	BIND_CONSTANT(SQLITE_CONSTRAINT);
	BIND_CONSTANT(SQLITE_MISMATCH);
	BIND_CONSTANT(SQLITE_MISUSE);
	BIND_CONSTANT(SQLITE_NOLFS);
	BIND_CONSTANT(SQLITE_AUTH);
	BIND_CONSTANT(SQLITE_FORMAT);
	BIND_CONSTANT(SQLITE_RANGE);
	BIND_CONSTANT(SQLITE_NOTADB);
	BIND_CONSTANT(SQLITE_NOTICE);
	BIND_CONSTANT(SQLITE_WARNING);
	BIND_CONSTANT(SQLITE_ROW);
	BIND_CONSTANT(SQLITE_DONE);
}

