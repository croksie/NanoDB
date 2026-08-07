#pragma once
#include <vector>
#include <memory>
#include "Column.h"
#include "Page.h"
#include "DataType.h"
#include "DataValue.h"
#include "Tuple.h"
#include "BufferManager.h"

class Table
{
private:
	int firstPageIndex = 0;
	std::string name;
	std::vector<Column> columns;

	std::shared_ptr<Page> loadedPage;

	BufferManager& bm;

public:
	Table(std::string name, std::vector<Column> column, int firstPageIndex, BufferManager& bm);

	std::string getName() const;
	const std::vector<Column>& getColumns() const { return this->columns; }

	void insertTuple(std::vector<std::unique_ptr<DataValue>> data);
	void insertTupleIntoPage(Tuple& tuple);
	Tuple createTuple(int slotIndex);

	std::vector<Tuple> getTuples();
	std::vector<Tuple> searchTuple(std::vector<std::string> cols, std::vector<std::shared_ptr<DataValue>> datas);

	void displayTable();
	// TODO : Create a function that return the first free space by passing the size of the space in parameter
	void nextPage();
	void prevPage();
};

