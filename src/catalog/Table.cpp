#include "Table.h"
#include "DatabaseException.h"


Table::Table(std::string name, std::vector<Column> column, int firstPageIndex, BufferManager& bm) 
	: m_columns(column), m_name(name), m_bm(bm), m_firstPageIndex(firstPageIndex) 
{
	try {
		m_loadedPage = m_bm.getPage(m_firstPageIndex);
	} catch (...) {
		std::shared_ptr<Page> newPage = std::make_shared<Page>(m_firstPageIndex, 0);
		m_bm.saveNewPage(newPage);
		m_loadedPage = m_bm.getPage(m_firstPageIndex);
	}
}

std::string Table::getName() const {
	return m_name;
}


void Table::insertTuple(std::vector<std::unique_ptr<DataValue>> data) {
	Tuple tuple = Tuple::serialize(data, m_columns);
	insertTupleIntoPage(tuple);
}

void Table::insertTupleIntoPage(Tuple& tuple)
{
	try
	{
		m_loadedPage->insertTuple(tuple);
	}
	catch (const DatabaseException&) // No more space in the current page
	{
		this->nextPage();
		this->insertTupleIntoPage(tuple);
	}
}

Tuple Table::createTuple(int slotIndex)
{
	std::vector<uint8_t> data = m_loadedPage->getTuple(slotIndex);

	return Tuple(data);
}

std::vector<Tuple> Table::getTuples() {
	m_loadedPage = m_bm.getPage(m_firstPageIndex);

	std::vector<Tuple> result;
	int nextPageId = 0;
	do {
		if (nextPageId != 0) this->nextPage();
		for (int i = 0; i < m_loadedPage->m_slotCount; ++i) {
			Tuple tuple = Tuple(m_loadedPage->getTuple(i));
			result.push_back(tuple);
		}
		nextPageId = m_loadedPage->m_nextPageId;
	} while (nextPageId != 0);

	return result;
}

void Table::displayTable()
{
	m_loadedPage = m_bm.getPage(m_firstPageIndex);
	int nextPageId = 0;
	do {
		if (nextPageId != 0) this->nextPage();
		for (int i = 0; i < m_loadedPage->m_slotCount; ++i) {
			std::vector<uint8_t> tuple = m_loadedPage->getTuple(i);

			size_t offset = 0;
			for (size_t colIdx = 0; colIdx < m_columns.size(); colIdx++) {
				Column c = m_columns.at(colIdx);
				std::unique_ptr<DataValue> valuePtr = c.getDataType()->deserialize(tuple, offset);
				std::cout << valuePtr->toString() << " ";
			}
			std::cout << std::endl;
		}
		nextPageId = m_loadedPage->m_nextPageId;
	} while (nextPageId != 0);
}

void Table::nextPage()
{
	int nextPageId = m_loadedPage->m_nextPageId;
	if (nextPageId == 0) {
		int newPageId = m_bm.allocatePage();
		std::shared_ptr<Page> newPage = std::make_shared<Page>(newPageId, m_loadedPage->m_pageId);
		m_loadedPage->m_nextPageId = newPageId;
		m_bm.saveNewPage(newPage);
		nextPageId = newPageId;
	}
	m_bm.flushPage(m_loadedPage->m_pageId);
	m_loadedPage = m_bm.getPage(nextPageId);
}

void Table::prevPage()
{
	int prevPageId = m_loadedPage->m_prevPageId;
	if (prevPageId != 0) {
		m_bm.flushPage(m_loadedPage->m_pageId);
		m_loadedPage = m_bm.getPage(prevPageId);
	}
}
