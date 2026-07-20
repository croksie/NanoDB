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
    try {
        std::cout << "--- Initialisation de la base de donnees ---" << std::endl;
        Database db;

     

        Table* utilisateur = db.getTable("Utilisateurs");
        if (utilisateur == nullptr) {
            std::cout << "La table n'existe pas. Creation de la table 'Utilisateurs'..." << std::endl;

            std::vector<Column> cols;
            cols.push_back(Column(std::make_shared<IntType>(), "Id"));
            cols.push_back(Column(std::make_shared<StringType>(), "name"));
            cols.push_back(Column(std::make_shared<IntType>(), "note"));

            db.createTable("Utilisateurs", cols);

            utilisateur = db.getTable("Utilisateurs");

            std::cout << "Insertion des tuples de test..." << std::endl;
            utilisateur->insertTuple(row(std::make_unique<IntValue>(1), std::make_unique<StringValue>("John"), std::make_unique<IntValue>(20)));
            utilisateur->insertTuple(row(std::make_unique<IntValue>(2), std::make_unique<StringValue>("Simon"), std::make_unique<IntValue>(10)));
            utilisateur->insertTuple(row(std::make_unique<IntValue>(3), std::make_unique<StringValue>("Martin"), std::make_unique<IntValue>(55)));
        } else {
            std::cout << "La table 'Utilisateurs' existe deja dans le catalogue et a ete chargee avec succes !" << std::endl;
        }

        std::cout << "--- Affichage de la table ---" << std::endl;
        utilisateur->displayTable();

        db.stats();

        std::cout << "Sauvegarde (flush) des donnees sur disque..." << std::endl;
        db.saveTable("Utilisateurs");

    } catch (const std::exception& e) {
        std::cerr << "Erreur standard capturee : " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Une erreur inattendue est survenue." << std::endl;
    }

    return 0;
}
