#pragma once
#include <vector>
struct Tuple
{
	std::vector<std::byte> data;

	Tuple(std::vector<std::byte> data) : data(data) {}
};

