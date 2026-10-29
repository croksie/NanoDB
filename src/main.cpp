#include <iostream>
#include <memory>


#include "Lexer.h"
#include "Parser.h"
#include "Executor.h"

#include "Database.h"
#include "DataType.h"
#include "DatabaseException.h"
#include "Utils.h"

int parseCommand(Database& db, const std::string& command) {

    if (command == ".exit") {
        return 2;
    }
    else if (command == ".tables") {
        Executor executor(db, "SELECT name FROM Systable;");
        return 0;
    }
    else if (command == ".help") {
        std::cout << "Commands : \n.tables : display all tables name\n.exit : exit the console\n.help : show this help" << std::endl;
        return 0;
    }
    else {
        return 1;
    }

}

int main() {
    try {
        std::cout << "--- Initialisation de la base de donnees ---" << std::endl;
        Database db;



        std::string command;

        while (true) {
            std::cout << "NanoDB > ";
            if (!std::getline(std::cin, command)) break;
            if (command.find_first_not_of(" \t\r\n") == std::string::npos) {
                continue;
            }
            int code = parseCommand(db, command);
            if (code == 2) {
                std::cout << "Goodbye" << std::endl;
                break;
            }
            else if (code == 1) {
                try {
                    Executor(db, command);
                }
                catch (const std::exception& e) {
                    std::cout << "Erreur standard capturee : " << e.what() << std::endl;
                }
                catch (...) {
                    std::cout << "Une erreur inattendue est survenue." << std::endl;
                }
            }
            else {
                continue;
            }


        }

    }
    catch (const std::exception& e) {
        std::cerr << "Erreur standard capturee : " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Une erreur inattendue est survenue." << std::endl;
    }

    return 0;
}
