#pragma once
#include <vector>
#include <any>

class Tuple
{
	int rowid;
public:
	std::vector<std::any> data;

public:
	Tuple(std::vector<std::any> data) : data(data) {}
};

