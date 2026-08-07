#include <iostream>
#include <memory>


#include "Lexer.h"
#include "Parser.h"
#include "Executor.h"

int main() {


    Lexer lexer = Lexer("Select * fRom utilisateurs;");

    std::vector<Token> tokens = lexer.Analyse();
    Parser parser = Parser(tokens);
    Executor executor = Executor(*parser.parseSelect());

    return 0;
}
