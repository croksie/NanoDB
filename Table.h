#pragma once
#include <vector>
#include <memory>
#include "Column.h"
#include "Page.h"
#include "DataType.h"
#include "DataValue.h"
#include "Tuple.h"

class Table
{
private:
	//int firstPageIndex = 0;
	std::string name;
	std::vector<Column> columns;
	std::vector<Tuple> tuples;


	//std::vector<Page> pages;

public:
	Table(std::string name, std::vector<Column> column);

	std::string getName() const;

	void addTuple(std::vector<std::unique_ptr<DataValue>> data);


	void displayTable();
};

