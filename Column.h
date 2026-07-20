#pragma once
#include <string>
#include <memory>
#include "DataType.h"

class Column
{
	std::string name;
	std::shared_ptr<DataType> data;

	//Constraints

	bool isNullable = false;

public:
	Column(std::shared_ptr<DataType> dataType, std::string name) : data(dataType), name(name) {}
	~Column() = default;

	std::shared_ptr<DataType> getDataType() const { return this->data; }
	std::string getName() const { return this->name; }
	bool getIsNullable() const { return this->isNullable; }

	std::vector<uint8_t> serialize();
};
