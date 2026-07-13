#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "BufferManager.h"
#include "Table.h"



class Database
{
public: 
	BufferManager bm = BufferManager();

	void clear();
	void stats();


	Table* getTable(std::string name);
	void createTable(std::string name, std::vector<Column> collumn);
	void deleteTable(std::string name);
private:
	std::vector<std::unique_ptr<Table>> tables;
};

