#include "Table.h"
#include "DatabaseException.h"


Table::Table(std::string name, std::vector<Column> column, int firstPageIndex, BufferManager& bm) 
	: columns(column), name(name), bm(bm), firstPageIndex(firstPageIndex) 
{
	try {
		this->loadedPage = this->bm.getPage(firstPageIndex);
	} catch (...) {
		std::shared_ptr<Page> newPage = std::make_shared<Page>(firstPageIndex, 0);
		this->bm.saveNewPage(newPage);
		this->loadedPage = this->bm.getPage(firstPageIndex);
	}
}

std::string Table::getName() const {
	return this->name;
}


void Table::insertTuple(std::vector<std::unique_ptr<DataValue>> data) {
	Tuple tuple = Tuple::serialize(data, this->columns);
	insertTupleIntoPage(tuple);
}

void Table::insertTupleIntoPage(Tuple& tuple)
{
	try
	{
		this->loadedPage->insertTuple(tuple);
	}
	catch (const DatabaseException&) // No more space in the current page
	{
		this->nextPage();
		this->insertTupleIntoPage(tuple);
	}
}

Tuple Table::createTuple(int slotIndex)
{
	std::vector<uint8_t> data = this->loadedPage->getTuple(slotIndex);

	return Tuple(data);
}

std::vector<Tuple> Table::getTuples() {
	this->loadedPage = this->bm.getPage(firstPageIndex);

	std::vector<Tuple> result;
	int nextPageId = 0;
	do {
		if (nextPageId != 0) this->nextPage();
		for (int i = 0; i < this->loadedPage->slotCount; ++i) {
			Tuple tuple = Tuple(this->loadedPage->getTuple(i));
			result.push_back(tuple);
		}
		nextPageId = this->loadedPage->nextPageId;
	} while (nextPageId != 0);

	return result;
}


std::vector<Tuple> Table::searchTuple(std::vector<std::string> cols, std::vector<std::shared_ptr<DataValue>> datas) {
	this->loadedPage = this->bm.getPage(firstPageIndex);

	std::vector<Tuple> result;
	int nextPageId = 0;
	do {
		if (nextPageId != 0) this->nextPage();
		for (int i = 0; i < this->loadedPage->slotCount; ++i) {
			Tuple tuple = Tuple(this->loadedPage->getTuple(i));
			std::vector<std::shared_ptr<DataValue>> values = tuple.deserialize(this->columns);
			size_t numberOfValue = cols.size();
			for (int i = 0; i < this->columns.size(); ++i) {
				for (int j = 0; j < cols.size(); ++j) {
					if (this->columns.at(i).getName() == cols.at(j)) {
						if ((*datas.at(j)).toString() == (*values.at(i)).toString()) {
							numberOfValue -= 1;
						}
					}
				}
			}
			if (numberOfValue == 0) {
				result.push_back(tuple);
			}
		}
		nextPageId = this->loadedPage->nextPageId;
	} while (nextPageId != 0);

	return result;
}

void Table::displayTable()
{
	this->loadedPage = this->bm.getPage(firstPageIndex);
	int nextPageId = 0;
	do {
		if (nextPageId != 0) this->nextPage();
		for (int i = 0; i < this->loadedPage->slotCount; ++i) {
			std::vector<uint8_t> tuple = this->loadedPage->getTuple(i);

			size_t offset = 0;
			for (size_t colIdx = 0; colIdx < this->columns.size(); colIdx++) {
				Column c = this->columns.at(colIdx);
				std::unique_ptr<DataValue> valuePtr = c.getDataType()->deserialize(tuple, offset);
				std::cout << valuePtr->toString() << " ";
			}
			std::cout << std::endl;
		}
		nextPageId = this->loadedPage->nextPageId;
	} while (nextPageId != 0);
}

void Table::nextPage()
{
	int nextPageId = this->loadedPage->nextPageId;
	if (nextPageId == 0) {
		int newPageId = this->bm.allocatePage();
		std::shared_ptr<Page> newPage = std::make_shared<Page>(newPageId, this->loadedPage->pageId);
		this->loadedPage->nextPageId = newPageId;
		this->bm.saveNewPage(newPage);
		nextPageId = newPageId;
	}
	this->bm.flushPage(this->loadedPage->pageId);
	this->loadedPage = this->bm.getPage(nextPageId);
}

void Table::prevPage()
{
	int prevPageId = this->loadedPage->prevPageId;
	if (prevPageId != 0) {
		this->bm.flushPage(this->loadedPage->pageId);
		this->loadedPage = this->bm.getPage(prevPageId);
	}
}
