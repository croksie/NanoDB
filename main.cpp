#include <iostream>
#include "Database.h"
#include "DataValue.h"
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

    DataValue col = new StringValue("Row1");
    db.createTable("Personne", {new StringValue("Row1")});



    //code



    db.deleteTable("Personne");
    

    return 0;
}