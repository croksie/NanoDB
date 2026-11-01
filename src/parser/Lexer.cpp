#include "Lexer.h"

std::vector<Token> Lexer::Analyse()
{
	while (m_current_pos < m_input.length()) {
		char c = m_input.at(m_current_pos);

		if (std::isspace(c)) {
			m_current_pos++;
			continue;
		}

		if (std::isalpha(c) || c == '_') {
			size_t startPos = m_current_pos;
			while (m_current_pos < m_input.length() && 
				   (std::isalnum(m_input[m_current_pos]) || m_input[m_current_pos] == '_')) {
				m_current_pos++;
			}
			std::string word = m_input.substr(startPos, m_current_pos - startPos);
			TokenType tt = matchKeyword(word);

			m_tokens.emplace_back(Token(tt, word));
			continue;
		}

		if (std::isdigit(c)) {
			size_t startPos = m_current_pos;
			while (m_current_pos < m_input.length() && std::isdigit(m_input[m_current_pos])) {
				m_current_pos++;
			}
			std::string numStr = m_input.substr(startPos, m_current_pos - startPos);
			m_tokens.emplace_back(Token(TokenType::NUMBER, numStr));
			continue;
		}

		if (c == '\'') {
			m_current_pos++;
			size_t startPos = m_current_pos;
			while (m_current_pos < m_input.length() && 
				   m_input[m_current_pos] != '\'') {
				m_current_pos++;
			}
			std::string varcharStr = m_rawInput.substr(startPos, m_current_pos - startPos);

			if (m_current_pos < m_input.length() && m_input[m_current_pos] == '\'') {
				m_current_pos++;
			}

			m_tokens.emplace_back(Token(TokenType::VARCHAR, varcharStr));
			continue;
		}

		if (c == ',') {
			m_tokens.emplace_back(Token(TokenType::COMMA, ","));
			m_current_pos++;
			continue;
		}
		if (c == ';') {
			m_tokens.emplace_back(Token(TokenType::SEMICOLON, ";"));
			m_current_pos++;
			continue;
		}
		if (c == '=') {
			m_tokens.emplace_back(Token(TokenType::EQUAL, "="));
			m_current_pos++;
			continue;
		}
		if (c == '>') {
			if (m_input.at(m_current_pos + 1) == '=') {
				m_tokens.emplace_back(Token(TokenType::GREATER_EQUAL, ">="));
				m_current_pos++;
			}
			else {
				m_tokens.emplace_back(Token(TokenType::GREATER, ">"));
			}
			m_current_pos++;
			continue;
		}
		if (c == '<') {
			if (m_input.at(m_current_pos + 1) == '=') {
				m_tokens.emplace_back(Token(TokenType::LESS_EQUAL, "<="));
				m_current_pos++;
			}
			else {
				m_tokens.emplace_back(Token(TokenType::LESS, "<"));
			}
			m_current_pos++;
			continue;
		}
		if (c == '!') {
			if (m_input.at(m_current_pos + 1) == '=') {
				m_tokens.emplace_back(Token(TokenType::NOT_EQUAL, "!="));
				m_current_pos++;
			}
			else {
				m_tokens.emplace_back(Token(TokenType::NOT, "!"));
			}
			m_current_pos++;
			continue;
		}
		if (c == '*') {
			m_tokens.emplace_back(Token(TokenType::STAR, "*"));
			m_current_pos++;
			continue;
		}
		if (c == '(') {
			m_tokens.emplace_back(Token(TokenType::LEFT_PARENTHESIS, "("));
			m_current_pos++;
			continue;
		}
		if (c == ')') {
			m_tokens.emplace_back(Token(TokenType::RIGHT_PARENTHESIS, ")"));
			m_current_pos++;
			continue;
		}


		m_current_pos++;
	}

	return m_tokens;
}
