#pragma once
#include <vector>
#include "Column.h"
#include "Page.h"
#include "DataType.h"
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

	void addTuple(std::vector<std::any> data);


	void displayTable();
	std::string anyToString(const std::any& a);
};

