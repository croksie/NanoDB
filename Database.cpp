#include "Database.h"
#include "DatabaseException.h"
#include <iostream>


//void Database::set(std::string key, DataValue* value) {
//	this->datas.insert_or_assign(key, value);
//}
//
//DataValue* Database::get(std::string key)  {
//	if (this->datas.contains(key)) {
//		auto val = this->datas.find(key);
//		return val->second;
//	}
//	else throw new DatabaseException("Pas dans la base");
//}
//
//bool Database::remove(std::string key) {
//	if (this->datas.contains(key)) {
//		datas.erase(key);
//		return 1;
//	}
//	return 0;
//}


void Database::clear() {
	this->tables.clear();
}

void Database::stats() {
	std::cout << "Il y a " << this->tables.size() << " tables dans la db." << std::endl;
	//std::cout << "Cela fait approximativement " << this->datas.size() * 8 << " octets." << std::endl;
}



Table* Database::getTable(std::string name)
{
	for (auto& t : this->tables) {
		if (t->getName() == name) {
			return t.get();
		}
	}
	return nullptr;
}

void Database::createTable(std::string name, std::vector<Column> collumn) {
	this->tables.push_back(std::make_unique<Table>(name, collumn, this->bm));
}

void Database::deleteTable(std::string name) {

	for (auto it = this->tables.begin(); it != this->tables.end();) {
		if ((*it)->getName() == name) {
			it = this->tables.erase(it);
		}
		else{
			it++;
		}
	}
}