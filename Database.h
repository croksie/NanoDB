#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Table.h"



class Database
{
public: 
	void clear();
	void stats();


	Table* getTable(std::string name);
	void createTable(std::string name, std::vector<Column> collumn);
	void deleteTable(std::string name);
private:
	std::vector<Table> tables;
};

