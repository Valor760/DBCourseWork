#include "database.h"
#include "constants.h"

namespace DB {
DataBase::~DataBase() {
	sqlite3_close(m_DB);
}

void DataBase::init(std::string db_path) {
	if(db_path.empty()) {
		// FIXME: Change to logging
		db_path = "database.db";
		std::cout << "ERROR: Provided path is empty! Using default \"" << db_path << "\"\n";
	}

	int result = sqlite3_open(db_path.c_str(), &m_DB);
	if(result != SQLITE_OK)
		throw std::runtime_error("Cannot open or create database on path: " + db_path);
	
	create_tables();
}

void DataBase::init() {
	init("");
}

int DataBase::callback(void* data, int argc, char** argv, char** column_name) {
	if(argc > 0) {
		if(m_LastQuery_Columns.empty()) {
			// Fill column names if vector is empty
			for(int i = 0; i < argc; i++) {
				m_LastQuery_Columns.push_back(std::string(column_name[i]));
			}
		}

		std::vector<std::string> curr_row;
		for(int i = 0; i < argc; i++) {
			curr_row.push_back(std::string(argv[i]));
		}
		m_LastQuery_Values.push_back(curr_row);
		return 0;
	}
	return -1;
}

bool DataBase::execute(const std::string query, ...) {
	if(query.empty()) {
		std::cout << "ERROR: Execution query is empty!\n";
		return false;
	}

	empty_last_query();
	
	char buff[1024];
	va_list ap;
	va_start(ap, query);
	vsnprintf(buff, 1024, query.c_str(), ap);
	va_end(ap);

	char* error_msg = nullptr;
	if(sqlite3_exec(m_DB, buff, callback, nullptr, &error_msg) != SQLITE_OK) {
		std::cout << "An error while executing SQL statement occurred!\n"
			<< "SQL: " << query << '\n'
			<< "Error msg: " << error_msg << '\n';
		sqlite3_free(error_msg);
		return false;
	}
	else {
		return true;
	}
}

auto& DataBase::get_last_query_result() const {
	return m_LastQuery_Values;
}

auto& DataBase::get_last_query_columns() const {
	return m_LastQuery_Columns;
}

void DataBase::empty_last_query() {
	// Empty query column names
	while(!m_LastQuery_Columns.empty())
		m_LastQuery_Columns.pop_back();
	
	// Empty received values
	while(!m_LastQuery_Values.empty())
		m_LastQuery_Values.pop_back();

	std::cout << "Emptied last query\n";
}

void DataBase::create_tables() {
	for(auto& query : CONSTS::ALL_CREATE_TABLES) {
		if(!execute(query)) {
			throw std::runtime_error("Error creating tables!");
		}
	}
}
} // namespace DB