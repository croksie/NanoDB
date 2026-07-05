#pragma once
#include <vector>
#include "Column.h"
#include "Page.h"
#include "DataValue.h"
class Table
{
private:
	int firstPageIndex = 0;
	std::string name;
	std::vector<Column> column;
	std::vector<Page> pages;

public:
	Table(std::string name, std::vector<Column> column);

	std::string getName() const;

	bool addTuple(std::vector<DataValue> datas);
};

