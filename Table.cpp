#include "Table.h"
#include "DatabaseException.h"



Table::Table(std::string name, std::vector<Column> column) : column(column), name(name) {

}


std::string Table::getName() const {
	return this->name;
}


bool Table::addTuple(std::vector<DataValue> datas) {
	if (datas.size() != this->column.size()) {
		throw new DatabaseException("Les donées fournie sont imcompatibles avec la structure de la table");
	}
}