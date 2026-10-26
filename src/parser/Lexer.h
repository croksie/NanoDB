#pragma once
#include <vector>
#include <string>
#include <cctype>

#include "Token.h"
#include "Utils.h"


class Lexer {

private:
    std::vector<Token> tokens;
    std::string input;
    std::string rawInput;
    size_t current_pos = 0;

public:
    Lexer(std::string input) : input(lowerStr(input)), rawInput(input) {};

    std::vector<Token> Analyse();

};