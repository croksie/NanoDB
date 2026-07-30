#pragma once
#include <string>
#include <unordered_map>

enum class TokenType {
    KEYWORD_SELECT, KEYWORD_FROM, KEYWORD_WHERE, 
    KEYWORD_INSERT, KEYWORD_INTO, KEYWORD_VALUES,
    KEYWORD_CREATE, KEYWORD_TABLE, KEYWORD_DROP,
    IDENTIFIER, NUMBER, VARCHAR, COMMA, SEMICOLON,
    EQUAL, GREATER, AND, OR, STAR, END_OF_FILE
};


inline TokenType matchKeyword(const std::string& word) {
	static const std::unordered_map<std::string, TokenType> keywords = {
		{"select", TokenType::KEYWORD_SELECT},
		{"from",   TokenType::KEYWORD_FROM},
		{"where",  TokenType::KEYWORD_WHERE},
		{"insert", TokenType::KEYWORD_INSERT},
		{"into",   TokenType::KEYWORD_INTO},
		{"values", TokenType::KEYWORD_VALUES},
		{"create", TokenType::KEYWORD_CREATE},
		{"table",  TokenType::KEYWORD_TABLE},
		{"drop",   TokenType::KEYWORD_DROP},
		{"and",   TokenType::AND},
		{"or",    TokenType::OR}
	};

	auto it = keywords.find(word);
	return (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
}


struct Token {
	TokenType type;
	std::string value;

	Token(TokenType type, std::string value) : type(type), value(value) {}
};