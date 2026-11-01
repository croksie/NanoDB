#include "Database.h"
#include "DatabaseException.h"
#include <iostream>

Database::Database() {
	loadSystemsTables();
}

void Database::clear() {
	m_tables.clear();
}

void Database::stats() {
	std::cout << "There are " << m_tables.size() << " tables in the db." << std::endl;
}

static std::string toLowerStr(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	return s;
}

Table* Database::getTable(std::string name)
{
	for (auto& [tName, tablePtr] : m_tables) {
		if (toLowerStr(tName) == toLowerStr(name)) {
			return tablePtr.get();
		}
	}
	try
	{
		this->loadTable(name);
		for (auto& [tName, tablePtr] : m_tables) {
			if (toLowerStr(tName) == toLowerStr(name)) {
				return tablePtr.get();
			}
		}
		return nullptr;
	}
	catch (const DatabaseException&)
	{
		return nullptr;
	}
}


void Database::createTable(std::string name, std::vector<Column> collumn) {
	int firstPageIndex = m_bm.allocatePage();
	m_tables[name] = std::make_unique<Table>(name, collumn, firstPageIndex, m_bm);
	m_tables["Systable"]->insertTuple(row(std::make_unique<StringValue>(name), std::make_unique<IntValue>(firstPageIndex)));

	for (Column c : collumn) {
		std::string type = (c.getDataType()->getTypeName() == Type::Number) ? "Number" : "Varchar";
		m_tables["Syscols"]->insertTuple(
			row(std::make_unique<StringValue>(name), 
				std::make_unique<StringValue>(c.getName()),
				std::make_unique<StringValue>(type),
				std::make_unique<IntValue>(c.getIsNullable() ? 1 : 0)
			));
	}
}

void Database::deleteTable(std::string name) {
	for (auto it = m_tables.begin(); it != m_tables.end(); ++it) {
		if (toLowerStr(it->first) == toLowerStr(name)) {
			m_tables.erase(it);
			break;
		}
	}
}





void Database::loadTable(std::string name) {
    Table* sysTables = m_tables.at("Systable").get();
    Table* sysCols = m_tables.at("Syscols").get();


	// Retrieve firstPageIndex
    int tableFirstPageIndex = -1;
	std::vector<Tuple> sysTableTuple = sysTables->getTuples();
    for (const auto& tuple : sysTableTuple) {
		std::vector<std::shared_ptr<DataValue>> row = tuple.deserialize(sysTables->getColumns());
        std::string tableName = std::any_cast<std::string>(row.at(0)->getValue());
        if (toLowerStr(tableName) == toLowerStr(name)) {
            tableFirstPageIndex = std::any_cast<int>(row.at(1)->getValue());
            break;
        }
    }

    if (tableFirstPageIndex == -1) {
        throw DatabaseException("Table not found in the database");
    }

	// Retrieve columns
    std::vector<Column> tableCols;
    std::vector<Tuple> sysColsRows = sysCols->getTuples();
    for (const auto& tuple : sysColsRows) {
		std::vector<std::shared_ptr<DataValue>> row = tuple.deserialize(sysCols->getColumns());
        std::string tableName = std::any_cast<std::string>(row.at(0)->getValue());
        if (toLowerStr(tableName) == toLowerStr(name)) {
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
        throw DatabaseException("Table columns not found in the database");
    }

    // Create Table object
    m_tables[name] = std::make_unique<Table>(name, tableCols, tableFirstPageIndex, m_bm);
}


void Database::loadSystemsTables()
{
	std::vector<Column> SystableCols;
	SystableCols.push_back(Column(std::make_shared<StringType>(), "name"));
	SystableCols.push_back(Column(std::make_shared<IntType>(), "firstPageIndex"));

	m_tables["Systable"] = std::make_unique<Table>("Systable", SystableCols, 0, m_bm);

	std::vector<Column> SyscolsCols;
	SyscolsCols.push_back(Column(std::make_shared<StringType>(), "tableName"));
	SyscolsCols.push_back(Column(std::make_shared<StringType>(), "name"));
	SyscolsCols.push_back(Column(std::make_shared<StringType>(), "dataType"));
	SyscolsCols.push_back(Column(std::make_shared<IntType>(), "isNullable"));

	m_tables["Syscols"] = std::make_unique<Table>("Syscols", SyscolsCols, 1, m_bm);
}