#include "Table.h"
#include "DatabaseException.h"



Table::Table(std::string name, std::vector<Column> column) : columns(column), name(name) {

}


std::string Table::getName() const {
	return this->name;
}


//void Table::addTuple(std::vector<std::unique_ptr<DataValue>> data) {
//	std::vector<std::byte> result;
//	for (auto i = 0; i < this->columns.size(); i++) {
//		Column c = this->columns.at(i);
//		if (c.getDataType()->getTypeName() != data.at(i)->type) {
//			throw new DatabaseException("Le type ne conrrespond pas à la structure de la table");
//		}
//		else {
//			std::vector<std::byte> tmp = c.getDataType()->serialize(data.at(i)->getValue());
//			result.insert(result.end(), tmp.begin(), tmp.end());
//		}
//	}
//
//	this->tuples.push_back(Tuple(result));
//}

void Table::addTuple(std::vector<std::unique_ptr<DataValue>> data) {
	std::vector<std::byte> result;
	for (auto i = 0; i < this->columns.size(); i++) {
		Column c = this->columns.at(i);
		if (c.getDataType()->getTypeName() != data.at(i)->type) {
			throw new DatabaseException("Le type ne conrrespond pas à la structure de la table");
		}
		else {
			std::vector<std::byte> tmp = c.getDataType()->serialize(data.at(i)->getValue());
			result.insert(result.end(), tmp.begin(), tmp.end());
		}
	}
	Tuple tuple = Tuple(result);
	this->page.insertTuple(tuple);

}

Tuple Table::createTuple(int slotIndex)
{
	std::vector<std::byte> data = this->page.getTuple(slotIndex);

	return Tuple(data);
}

//void Table::displayTable()
//{
//	for (Tuple& t : this->tuples) {
//		size_t offset = 0;
//		for (auto i = 0; i < this->columns.size(); i++) {
//			Column c = this->columns.at(i);
//			std::unique_ptr<DataValue> valuePtr = c.getDataType()->deserialize(t.data, offset);
//			std::cout << valuePtr->toString() << " ";
//		}
//		std::cout << std::endl;
//	}
//}

void Table::displayTable()
{
	for (int i = 0; i < this->page.slotCount; ++i) {
		std::vector<std::byte> tuple = this->page.getTuple(i);

		size_t offset = 0;
		for (auto i = 0; i < this->columns.size(); i++) {
			Column c = this->columns.at(i);
			std::unique_ptr<DataValue> valuePtr = c.getDataType()->deserialize(tuple, offset);
			std::cout << valuePtr->toString() << " ";
		}
		std::cout << std::endl;
	}
}