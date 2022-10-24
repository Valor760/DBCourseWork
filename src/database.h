#include "sqlite3.h"

#include <string.h>
#include <iostream>
#include <vector>

// #define TABLE_COLUMN_DELIMITER ";;;"


namespace DB {
class DataBase {
	public:
		DataBase() {}
		~DataBase();

		// Remove copy and move contructors
		DataBase(DataBase&&) = delete;
		DataBase& operator=(const DataBase&) = delete;

		// Initialize database and tables
		void init();
		void init(std::string db_path);
		int execute(const std::string& query);

		// Returns m_LastQuery_Values
		auto& get_last_query_result() const;
		// Returns m_LastQuery_Columns
		auto& get_last_query_columns() const;

		// Empties m_LastQuery_Columns and m_LastQuery_Values
		void empty_last_query();
	
	private:
		static int callback(void* data, int argc, char** argv, char** column_name);
		void create_tables();

	private:
		sqlite3* m_DB = nullptr;

		static inline std::vector<std::string> m_LastQuery_Columns;
		static inline std::vector<std::vector<std::string>> m_LastQuery_Values;
};
} // namespace DB