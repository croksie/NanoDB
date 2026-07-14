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
    cols.push_back(Column(std::make_shared<IntType>(), "note"));

    db.createTable("Utilisateurs", cols);

    Table* utilisateur = db.getTable("Utilisateurs");

    utilisateur->insertTuple(row(std::make_unique<IntValue>(1), std::make_unique<StringValue>("John"), std::make_unique<IntValue>(20)));
    utilisateur->insertTuple(row(std::make_unique<IntValue>(2), std::make_unique<StringValue>("Simon"), std::make_unique<IntValue>(10)));
    utilisateur->insertTuple(row(std::make_unique<IntValue>(3), std::make_unique<StringValue>("Martin"), std::make_unique<IntValue>(55)));

    utilisateur->displayTable();

    db.stats();

    db.deleteTable("Utilisateurs");
    

    return 0;
}

