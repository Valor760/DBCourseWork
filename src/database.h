#include "sqlite3.h"

#include <string.h>
#include <iostream>

namespace DB {
class DataBase {
	public:
		DataBase() {}
		~DataBase();

		// Remove copy and move contructors
		DataBase(DataBase&&) = delete;
		DataBase& operator=(const DataBase&) = delete;

		// Initialize database
		//
		// return 0 if success, -1 otherwise
		int init(std::string db_path);
	
	private:
		sqlite3* db = nullptr;
};
} // namespace DB