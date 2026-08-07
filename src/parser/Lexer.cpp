#include "Lexer.h"

std::vector<Token> Lexer::Analyse()
{
	while (this->current_pos < this->input.length()) {
		char c = this->input.at(this->current_pos);

		if (std::isspace(c)) {
			this->current_pos++;
			continue;
		}

		if (std::isalpha(c) || c == '_') {
			size_t startPos = this->current_pos;
			while (this->current_pos < this->input.length() && 
				   (std::isalnum(this->input[this->current_pos]) || this->input[this->current_pos] == '_')) {
				this->current_pos++;
			}
			std::string word = this->input.substr(startPos, this->current_pos - startPos);
			TokenType tt = matchKeyword(word);

			tokens.emplace_back(Token(tt, word));
			continue;
		}

		if (std::isdigit(c)) {
			size_t startPos = this->current_pos;
			while (this->current_pos < this->input.length() && std::isdigit(this->input[this->current_pos])) {
				this->current_pos++;
			}
			std::string numStr = this->input.substr(startPos, this->current_pos - startPos);
			tokens.emplace_back(Token(TokenType::NUMBER, numStr));
			continue;
		}

		if (c == '"') {
			size_t startPos = this->current_pos + 1;
			while (this->current_pos < this->input.length() && 
				   this->input[this->current_pos] != '\'') {
				this->current_pos++;
			}
			std::string varcharStr = this->input.substr(startPos, this->current_pos - startPos - 1);
			tokens.emplace_back(Token(TokenType::VARCHAR, varcharStr));

		}

		if (c == ',') {
			tokens.emplace_back(Token(TokenType::COMMA, ","));
			this->current_pos++;
			continue;
		}
		if (c == ';') {
			tokens.emplace_back(Token(TokenType::SEMICOLON, ";"));
			this->current_pos++;
			continue;
		}
		if (c == '=') {
			tokens.emplace_back(Token(TokenType::EQUAL, "="));
			this->current_pos++;
			continue;
		}
		if (c == '>') {
			if (this->input.at(this->current_pos + 1) == '=') {
				tokens.emplace_back(Token(TokenType::GREATER_EQUAL, ">="));
			}
			else {
				tokens.emplace_back(Token(TokenType::GREATER, ">"));
			}
			this->current_pos++;
			continue;
		}
		if (c == '<') {
			if (this->input.at(this->current_pos + 1) == '=') {
				tokens.emplace_back(Token(TokenType::LESS_EQUAL, "<="));
			}
			else {
				tokens.emplace_back(Token(TokenType::LESS, "<"));
			}
			this->current_pos++;
			continue;
		}
		if (c == '!') {
			if (this->input.at(this->current_pos + 1) == '=') {
				tokens.emplace_back(Token(TokenType::NOT_EQUAL, "!="));
			}
			else {
				tokens.emplace_back(Token(TokenType::NOT, "!"));
			}
			this->current_pos++;
			continue;
		}
		if (c == '*') {
			tokens.emplace_back(Token(TokenType::STAR, "*"));
			this->current_pos++;
			continue;
		}
		if (c == '(') {
			tokens.emplace_back(Token(TokenType::LEFT_PARENTHESIS, "("));
			this->current_pos++;
			continue;
		}
		if (c == ')') {
			tokens.emplace_back(Token(TokenType::RIGHT_PARENTHESIS, ")"));
			this->current_pos++;
			continue;
		}

		// TODO : Emplement LESS, LESS_EQUAL, GREATER_EQUAL, NOT_EQUAL and PARENTHESIS


		this->current_pos++;
	}

	return this->tokens;
}
