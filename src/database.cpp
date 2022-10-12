#include "database.h"


namespace DB {
DataBase::~DataBase() {
	sqlite3_close(db);
}

int DataBase::init(std::string db_path) {
	if(db_path.empty()) {
		// FIXME: Change to logging
		std::cout << "ERROR: Provided path is empty!\n";
		return -1;
	}

	int result = sqlite3_open(db_path.c_str(), &db);
	if(result != SQLITE_OK) {
		// FIXME: Change to logging
		std::cout << "ERROR: Cannot open DB\n";
		return -1;
	}

	return 0;
}
} // namespace DB