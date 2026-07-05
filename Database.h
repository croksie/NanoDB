#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Table.h"



class Database
{
public: 
	//void set(std::string key, DataValue* value);
	//DataValue* get(std::string key);

	//bool remove(std::string key);
	void clear();
	void stats();

	void createTable(std::string name, std::vector<Column> collumn);
	void deleteTable(std::string name);
private:
	//std::unordered_map<std::string, DataValue*> datas;
	std::vector<Table> tables;
};

