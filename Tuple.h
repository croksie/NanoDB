#pragma once
#include <vector>
#include <cstdint>
struct Tuple
{
	std::vector<uint8_t> data;

	Tuple(std::vector<uint8_t> data) : data(data) {}
};

