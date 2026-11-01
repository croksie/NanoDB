#pragma once
#include <vector>
#include <string>
#include <cctype>

#include "Token.h"
#include "Utils.h"


class Lexer {

private:
    std::vector<Token> m_tokens;
    std::string m_input;
    std::string m_rawInput;
    size_t m_current_pos = 0;

public:
    Lexer(std::string input) : m_input(lowerStr(input)), m_rawInput(input) {};

    std::vector<Token> Analyse();

};