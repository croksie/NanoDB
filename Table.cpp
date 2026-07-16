#include "Table.h"
#include "DatabaseException.h"



Table::Table(std::string name, std::vector<Column> column, BufferManager& bm) : columns(column), name(name), bm(bm) {
	this->firstPageIndex = this->bm.allocatePage();
	std::shared_ptr<Page> newPage = std::make_shared<Page>(firstPageIndex, 0);
	this->bm.saveNewPage(newPage);
	this->loadedPage = this->bm.getPage(firstPageIndex);
}

Table::Table(std::vector<uint8_t> data, BufferManager& bm) : bm(bm)
{
	size_t offset = 0;

	// Deserialize name length
	if (data.size() < sizeof(size_t)) {
		throw new DatabaseException("Table corrompu");
	}

	size_t nameLength = 0;
	for (size_t i = 0; i < sizeof(size_t); ++i) {
		nameLength |= static_cast<size_t>(data[offset++]) << (8 * i);
	}

	// Deserialize name
	if (offset + nameLength > data.size()) {
		throw new DatabaseException("Table corrompu");
	}

	name.reserve(nameLength);
	for (size_t i = 0; i < nameLength; ++i) {
		name.push_back(static_cast<char>(data[offset++]));
	}

	// Deserialize columns
	while (offset < data.size()) {
		size_t size = 0;
		for (size_t i = 0; i < sizeof(size_t); ++i) {
			size |= static_cast<size_t>(data[offset++]) << (8 * i);
		}
		std::vector<uint8_t> columnData(data.begin() + offset, data.begin() + offset + size);
		offset += size;
		Column c = Column(columnData);
		this->columns.push_back(c);
	}

	// Deserialize firstPageIndex
	if (offset + sizeof(int) > data.size()) {
		throw new DatabaseException("Table corrompu");
	}

	for (size_t i = 0; i < sizeof(int); ++i) {
		firstPageIndex |= static_cast<int>(data[offset++]) << (8 * i);
	}

	this->loadedPage = this->bm.getPage(firstPageIndex);
}

std::vector<uint8_t> Table::serialize()
{
	std::vector<uint8_t> result;

	// Serialize name
	size_t nameLength = name.size();
	result.reserve(sizeof(size_t) + nameLength + 2); // size + name bytes + 1 byte for type + 1 byte for nullable

	// Serialize name length
	for (size_t i = 0; i < sizeof(size_t); ++i) {
		result.push_back(static_cast<uint8_t>((nameLength >> (8 * i)) & 0xFF));
	}

	// Serialize name characters
	for (char c : name) {
		result.push_back(static_cast<uint8_t>(c));
	}

	// Serialize columns
	for (size_t i = 0; i < this->columns.size(); i++) {
		std::vector<uint8_t> tmp = this->columns.at(i).serialize();
		result.insert(result.end(), tmp.begin(), tmp.end());
	}

	// Serialize firstPageIndex
	for (size_t i = 0; i < sizeof(int); ++i) {
		result.push_back(static_cast<uint8_t>((firstPageIndex >> (8 * i)) & 0xFF));
	}

	return result;
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
	int nextPageId;
	do {
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
