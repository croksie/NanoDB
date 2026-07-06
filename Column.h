#pragma once
#include <string>
#include "DataType.h"

class Column
{
	const std::string name;
	const DataType* data;

	//Constraints

	const bool isNullable = false;

public:
	Column(DataType* dataType, std::string name) : data(dataType), name(name) {}
};

