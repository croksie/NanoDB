#include "Table.h"
#include "DatabaseException.h"



Table::Table(std::string name, std::vector<Column> column) : columns(column), name(name) {

}


std::string Table::getName() const {
	return this->name;
}


void Table::addTuple(std::vector<std::any> data) {
	this->tuples.push_back(Tuple(data));
}

void Table::displayTable()
{
	for (Tuple& t : this->tuples) {
		for (auto& data : t.data) {
			std::cout << this->anyToString(data) << " ";
		}
		std::cout << std::endl;
	}
}


std::string Table::anyToString(const std::any& a) {
    if (a.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(a));
    }
    else if (a.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(a));
    }
    else if (a.type() == typeid(std::string)) {
        return std::any_cast<std::string>(a);
    }
    else if (a.type() == typeid(const char*)) {
        return std::string(std::any_cast<const char*>(a));
    }
    // Ajoute d'autres types ici
    return "[unknown]";
}