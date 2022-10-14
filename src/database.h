#include "sqlite3.h"

#include <string.h>
#include <iostream>

#define TABLE_ARR_SIZE_LARGE 100
#define TABLE_ARR_SIZE_MEDIUM 20
#define TABLE_ARR_SIZE_SMALL 1
#define TABLE_COLUMN_DELIMITER ";;;"

namespace DB {
namespace Table {
struct Flights {
	int fl_id;
	char fl_city_takeoff[TABLE_ARR_SIZE_LARGE];
	char fl_city_land[TABLE_ARR_SIZE_LARGE];
	// FIXME: this should be a date
	int fl_date_start;
	double fl_duration_h;
};

struct Passengers {
	int p_id;
	char p_name[TABLE_ARR_SIZE_LARGE];
	char p_surname[TABLE_ARR_SIZE_LARGE];
	char p_gender[TABLE_ARR_SIZE_SMALL];
	char p_phone_num[TABLE_ARR_SIZE_MEDIUM];
};

struct Baggage {
	int b_id;
	double b_weight_kg;
	char b_type[5];
	bool b_is_fragile;
};

struct Employees {
	int emp_id;
	char emp_name[TABLE_ARR_SIZE_LARGE];
	char emp_surname[TABLE_ARR_SIZE_LARGE];
	char emp_gender[TABLE_ARR_SIZE_SMALL];
	char emp_phone_num[TABLE_ARR_SIZE_MEDIUM];
	char emp_position[TABLE_ARR_SIZE_MEDIUM];
	double emp_salary_eur;
};

struct Hangars {
	int h_id;
	int h_size;
};

struct Airplanes {
	char plane_id[10];
	int plane_capacity;
	char plane_type[3];
	char plane_company[TABLE_ARR_SIZE_LARGE];
};
} // namespace Table

enum TABLES {
	TABLE_FLIGHTS,
	TABLE_ONE_FLIGHT,
	TABLE_AIRPLANES,
	TABLE_PASSENGERS,
	TABLE_BAGGAGE,
	TABLE_HANGAR,
	TABLE_EMPLOYEES,
	TABLE_NONE
};

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
		int init(const std::string& db_path);

		int create(const std::string& query);
		
		auto get_last_result();
	
	private:
		int execute(const std::string& query, const TABLES& table = TABLE_NONE);
		static int callback(void* data, int argc, char** argv, char** column_name);

	private:
		sqlite3* m_DB = nullptr;

		static inline std::string last_query_result = "";
		enum TABLES last_query_table = TABLE_NONE;
};
} // namespace DB