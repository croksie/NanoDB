#pragma once
#include <vector>
#include <cstdint>

#include "Column.h"
#include "DataValue.h"



struct Tuple
{
	std::vector<uint8_t> data;

	Tuple(std::vector<uint8_t> data) : data(data) {}
	
	static Tuple serialize(const std::vector<std::unique_ptr<DataValue>>& values, const std::vector<Column>& columns);
	std::vector<std::shared_ptr<DataValue>> deserialize(const std::vector<Column>& columns) const;
};

