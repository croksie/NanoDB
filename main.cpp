#include <iostream>
#include "Database.h"
#include "DataType.h"
#include "DatabaseException.h"


//int main() {
//    Database db;
//
//    // Ajout de données
//    db.set("user:101:name", new StringValue("Alice"));
//    db.set("user:101:age", new IntValue(25));
//
//    try {
//        DataValue* val = db.get("user:101:name");
//        val->display(); // Devrait afficher: [String] Alice
//        val = db.get("user:101:age");
//        val->display();
//    }
//    catch (const DatabaseException& e) {
//        std::cerr << e.what() << std::endl;
//    }
//
//    db.stats();
//
//    return 0;
//    // À ce stade, TOUTE la mémoire doit être libérée automatiquement.
//}


int main() {
    Database db;

    std::vector<Column> cols;
    cols.push_back(Column(new IntType(), "Id"));
    cols.push_back(Column(new StringType(), "name"));

    db.createTable("Utilisateurs", cols);

    Table* utilisateur = db.getTable("Utilisateurs");

    utilisateur->addTuple({ 1,"John" });
    utilisateur->addTuple({ 2,"Simon" });
    utilisateur->addTuple({ 3,"Martin" });
    utilisateur->displayTable();
    //code

    db.stats();

    db.deleteTable("Personne");
    

    return 0;
}

