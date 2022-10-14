#include "database.h"


namespace DB {
DataBase::~DataBase() {
	sqlite3_close(m_DB);
}

int DataBase::init(const std::string& db_path) {
	if(db_path.empty()) {
		// FIXME: Change to logging
		std::cout << "ERROR: Provided path is empty!\n";
		return -1;
	}

	int result = sqlite3_open(db_path.c_str(), &m_DB);
	if(result != SQLITE_OK) {
		// FIXME: Change to logging
		std::cout << "ERROR: Cannot open DB\n";
		return -1;
	}


	return 0;
}

int DataBase::callback(void* data, int argc, char** argv, char** column_name) {
	// enum TABLES table_type = *(TABLES*)data;

	// if(argc > 0 || table_type == TABLE_NONE) {
	// 	switch(table_type) {
	// 		case TABLE_FLIGHTS:
				
	// 			break;
	// 		case TABLE_AIRPLANES:
	// 			break;
	// 		case TABLE_BAGGAGE:
	// 			break;
	// 		case TABLE_EMPLOYEES:
	// 			break;
	// 		case TABLE_HANGAR:
	// 			break;
	// 		case TABLE_ONE_FLIGHT:
	// 			break;
	// 		case TABLE_PASSENGERS:
	// 			break;
	// 		default:
	// 			std::cout << "Wrong table type\n";
	// 			return -1;
	// 	}
	// 	return 0;
	// }
	// else
	// 	return 0;
	if(argc > 0) {
		last_query_result = ""; // Flush previous content
		while(**argv) {
			last_query_result += **argv;
			last_query_result += TABLE_COLUMN_DELIMITER;
			*argv++;
		}
	}
	return 0;
}

int DataBase::create(const std::string& query) {
	return execute(query);
}

int DataBase::execute(const std::string& query, const TABLES& table) {
	if(query.empty()) {
		std::cout << "ERROR: Execution query is empty!\n";
		return -1;
	}

	char* error_msg = nullptr;
	if(sqlite3_exec(m_DB, query.c_str(), callback, (void*)table, &error_msg) != SQLITE_OK) {
		std::cout << "An error while executing SQL statement occurred!\n"
			<< "SQL: " << query << "\n"
			<< "Error msg: " << error_msg << std::endl;
		sqlite3_free(error_msg);
		return -1;
	}
	else {
		return 0;
	}
}

// Parse last query result and return appropriate structure
auto DataBase::get_last_result() {
	if(last_query_result.empty())
}
} // namespace DB

// future promise