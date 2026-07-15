#include "Table.h"
#include "DatabaseException.h"



Table::Table(std::string name, std::vector<Column> column, BufferManager& bm) : columns(column), name(name), bm(bm), loadedPage(bm.getPage(0)){
	//Page p = Page();
	//dm.writePage(p);
	//this->loadedPage = bm.getPage(0);
}

std::string Table::getName() const {
	return this->name;
}


void Table::insertTuple(std::vector<std::unique_ptr<DataValue>> data) {
	std::vector<uint8_t> result;
	for (size_t i = 0; i < this->columns.size(); i++) {
		Column c = this->columns.at(i);
		if (c.getDataType()->getTypeName() != data.at(i)->type) {
			throw new DatabaseException("Le type ne conrrespond pas à la structure de la table");
		}
		else {
			std::vector<uint8_t> tmp = c.getDataType()->serialize(data.at(i)->getValue());
			result.insert(result.end(), tmp.begin(), tmp.end());
		}
	}
	Tuple tuple = Tuple(result);
	insertTupleIntoPage(tuple);

}

void Table::insertTupleIntoPage(Tuple& tuple)
{
	try
	{
		this->loadedPage->insertTuple(tuple);
	}
	catch (const DatabaseException&)
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


void Table::displayTable()
{
	for (int i = 0; i < this->loadedPage->slotCount; ++i) {
		std::vector<uint8_t> tuple = this->loadedPage->getTuple(i);

		size_t offset = 0;
		for (size_t i = 0; i < this->columns.size(); i++) {
			Column c = this->columns.at(i);
			std::unique_ptr<DataValue> valuePtr = c.getDataType()->deserialize(tuple, offset);
			std::cout << valuePtr->toString() << " ";
		}
		std::cout << std::endl;
	}
}

void Table::nextPage()
{
	int pageId = this->loadedPage->nextPageId;
	if (pageId == 0) {
		int newPageId = this->bm.allocatePage();
		std::shared_ptr<Page> newPage = std::make_shared<Page>(newPageId, this->loadedPage->pageId);
		this->bm.saveNewPage(newPage);
	}
	this->bm.flushPage(this->loadedPage->pageId);
	this->loadedPage = this->bm.getPage(pageId);
}

void Table::prevPage()
{
	int pageId = this->loadedPage->prevPageId;
	if (pageId != 0) {
		this->bm.flushPage(this->loadedPage->pageId);
		this->loadedPage = this->bm.getPage(pageId);
	}
}
