#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "DataValue.h"


class Database
{
public: 
	void set(std::string key, DataValue* value);
	DataValue* get(std::string key);

	bool remove(std::string key);
	void clear();
	void stats();
private:
	std::unordered_map<std::string, DataValue*> datas;
};

