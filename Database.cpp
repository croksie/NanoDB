#include "Database.h"
#include "DatabaseException.h"
#include <iostream>


void Database::set(std::string key, DataValue* value) {
	this->datas.insert_or_assign(key, value);
}

DataValue* Database::get(std::string key)  {
	if (this->datas.contains(key)) {
		auto val = this->datas.find(key);
		return val->second;
	}
	else throw new DatabaseException("Pas dans la base");
}

bool Database::remove(std::string key) {
	if (this->datas.contains(key)) {
		datas.erase(key);
		return 1;
	}
	return 0;
}


void Database::clear() {
	this->datas.clear();
}

void Database::stats() {
	std::cout << "Il y a " << this->datas.size() << " elements dans la db." << std::endl;
	std::cout << "Cela fait approximativement " << this->datas.size() * 8 << " octets." << std::endl;
}