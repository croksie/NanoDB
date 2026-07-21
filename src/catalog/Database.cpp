#include "Database.h"
#include "DatabaseException.h"
#include <iostream>

Database::Database() {
	loadSystemsTables();
}

void Database::clear() {
	this->tables.clear();
}

void Database::stats() {
	std::cout << "Il y a " << this->tables.size() << " tables dans la db." << std::endl;
}

Table* Database::getTable(std::string name)
{
	if (this->tables.contains(name)) {
		return this->tables.at(name).get();
	}
	else {
		try
		{
			this->loadTable(name);
			return this->tables.at(name).get();
		}
		catch (const DatabaseException&)
		{
			return nullptr;
		}
	}
}


void Database::createTable(std::string name, std::vector<Column> collumn) {
	int firstPageIndex = this->bm.allocatePage();
	this->tables[name] = std::make_unique<Table>(name, collumn, firstPageIndex, this->bm);
	this->tables["Systable"]->insertTuple(row(std::make_unique<StringValue>(name), std::make_unique<IntValue>(firstPageIndex)));

	for (Column c : collumn) {
		std::string type = (c.getDataType()->getTypeName() == Type::Number) ? "Number" : "Varchar";
		this->tables["Syscols"]->insertTuple(
			row(std::make_unique<StringValue>(name), 
				std::make_unique<StringValue>(c.getName()),
				std::make_unique<StringValue>(type),
				std::make_unique<IntValue>(c.getIsNullable() ? 1 : 0)
			));
	}
}

void Database::deleteTable(std::string name) {
	if (this->tables.contains(name)) {
		this->tables.erase(name);
	}
}

void Database::saveTable(std::string name) {
	this->bm.flushAll();
}



void Database::loadTable(std::string name) {
    Table* sysTables = this->tables.at("Systable").get();
    Table* sysCols = this->tables.at("Syscols").get();


	// Retrieve firstPageIndex
    int tableFirstPageIndex = -1;
	std::vector<Tuple> sysTableTuple = sysTables->getTuples();
    for (const auto& tuple : sysTableTuple) {
		std::vector<std::shared_ptr<DataValue>> row = tuple.deserialize(sysTables->getColumns());
        std::string tableName = std::any_cast<std::string>(row.at(0)->getValue());
        if (tableName == name) {
            tableFirstPageIndex = std::any_cast<int>(row.at(1)->getValue());
            break;
        }
    }

    if (tableFirstPageIndex == -1) {
        throw DatabaseException("Table non trouvee dans le catalogue");
    }

	// Retrieve columns
    std::vector<Column> tableCols;
    std::vector<Tuple> sysColsRows = sysCols->getTuples();
    for (const auto& tuple : sysColsRows) {
		std::vector<std::shared_ptr<DataValue>> row = tuple.deserialize(sysCols->getColumns());
        std::string tableName = std::any_cast<std::string>(row.at(0)->getValue());
        if (tableName == name) {
            std::string colName = std::any_cast<std::string>(row.at(1)->getValue());
            std::string dataType = std::any_cast<std::string>(row.at(2)->getValue());
            
            std::shared_ptr<DataType> dt;
            if (dataType == "Number") {
                dt = std::make_shared<IntType>();
            } else {
                dt = std::make_shared<StringType>();
            }
            tableCols.push_back(Column(dt, colName));
        }
    }

    if (tableCols.empty()) {
        throw DatabaseException("Colonnes de la table non trouvees dans le catalogue");
    }

    // Create Table object
    this->tables[name] = std::make_unique<Table>(name, tableCols, tableFirstPageIndex, this->bm);
}


void Database::loadSystemsTables()
{
	std::vector<Column> SystableCols;
	SystableCols.push_back(Column(std::make_shared<StringType>(), "name"));
	SystableCols.push_back(Column(std::make_shared<IntType>(), "firstPageIndex"));

	this->tables["Systable"] = std::make_unique<Table>("Systable", SystableCols, 0, this->bm);

	std::vector<Column> SyscolsCols;
	SyscolsCols.push_back(Column(std::make_shared<StringType>(), "tableName"));
	SyscolsCols.push_back(Column(std::make_shared<StringType>(), "name"));
	SyscolsCols.push_back(Column(std::make_shared<StringType>(), "dataType"));
	SyscolsCols.push_back(Column(std::make_shared<IntType>(), "isNullable"));

	this->tables["Syscols"] = std::make_unique<Table>("Syscols", SyscolsCols, 1, this->bm);
}