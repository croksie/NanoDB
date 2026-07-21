#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "BufferManager.h"
#include "Table.h"
#include "Utils.h"


class Database
{
public: 
	BufferManager bm = BufferManager();

	Database();
	void clear();
	void stats();


	Table* getTable(std::string name);
	void createTable(std::string name, std::vector<Column> collumn);
	void deleteTable(std::string name);

	void saveTable(std::string name);
	void loadTable(std::string name);

	void loadSystemsTables();
private:
	std::unordered_map<std::string, std::unique_ptr<Table>> tables;
};

