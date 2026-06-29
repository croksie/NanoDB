#pragma once
#include <vector>
#include "Column.h"
class Table
{
private:
	int firstPageIndex = 0;
	std::vector<Column> column;

public:
	Table(std::vector<Column> column);
};

