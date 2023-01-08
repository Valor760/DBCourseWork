#pragma once

#include <vector>
#include <string>
#include <iostream>

typedef std::vector<std::vector<std::string>> 	TableRows;
typedef std::vector<std::string> 				TableCols;\

#define LOG_INFO	1
#define LOG_ERR		2

#ifdef _DEBUG
#define LOG(level, str)									\
	do {												\
		switch(level) {									\
			case LOG_INFO:								\
				std::cout << "INFO: " << str << "\n";	\
				break;									\
			case LOG_ERR:								\
				std::cerr << "ERROR: " << str << "\n";	\
				break;									\
			default:									\
				break;									\
		}												\
	} while(false);
#else
#define LOG(level, str)
#endif