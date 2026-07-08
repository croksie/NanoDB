#include <iostream>
#include <memory>
#include "Database.h"
#include "DataType.h"
#include "DatabaseException.h"

template <typename... Args>
auto row(Args&&... args) {
    std::vector<std::unique_ptr<DataValue>> r;
    (r.push_back(std::move(args)), ...);
    return r;
}

int main() {
    Database db;

    std::vector<Column> cols;
    cols.push_back(Column(std::make_shared<IntType>(), "Id"));
    cols.push_back(Column(std::make_shared<StringType>(), "name"));

    db.createTable("Utilisateurs", cols);

    Table* utilisateur = db.getTable("Utilisateurs");

    utilisateur->addTuple(row(std::make_unique<IntValue>(1), std::make_unique<StringValue>("John")));
    utilisateur->addTuple(row(std::make_unique<IntValue>(2), std::make_unique<StringValue>("Simon")));
    utilisateur->addTuple(row(std::make_unique<IntValue>(3), std::make_unique<StringValue>("Martin")));

    utilisateur->displayTable();

    db.stats();

    db.deleteTable("Utilisateurs");
    

    return 0;
}

