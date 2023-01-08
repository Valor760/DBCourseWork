#include "sqlite3.h"

#include "common.h"


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
		bool execute(const std::string query, ...);

		// Returns m_LastQuery_Values
		const TableRows& GetLastQueryResult() const;
		// Returns m_LastQuery_Columns
		const TableCols& GetLastQueryColumns() const;
		const std::string& GetLastErrorMsg() const;
		const TableRows& GetTableInfo(const std::string& table_name);

	
	private:
		static int callback(void* data, int argc, char** argv, char** column_name);
		void create_tables();
		// Empties m_LastQuery_Columns and m_LastQuery_Values
		void empty_last_query();

	private:
		sqlite3* m_DB = nullptr;

		static inline TableCols m_LastQuery_Columns;
		static inline TableRows m_LastQuery_Values;
		std::string m_LastErrorMsg = "";
};
} // namespace DB