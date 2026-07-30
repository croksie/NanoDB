#pragma once
#include <vector>
#include "Token.h"
#include "AST.h"




class Parser {

private:
	std::vector<Token> tokens;
	std::vector<ASTNode> nodes;

	size_t current = 0;

public:
	Parser(std::vector<Token> tokens);
	virtual ~Parser() = default;

	Token peek();
	Token advance();
	bool check(TokenType type);
	Token consume(TokenType type, std::string error_msg);

	std::unique_ptr<ASTNode>  parseStatement();
	std::unique_ptr<SelectStatement> parseSelect();
	std::unique_ptr<TableRef>   parseTableRef();
	std::unique_ptr<Expression> parseExpression();
	std::unique_ptr<Expression> parseOr();
	std::unique_ptr<Expression> parseAnd();
	std::unique_ptr<Expression> parseComparison();
	std::unique_ptr<Expression> parsePrimary();
};