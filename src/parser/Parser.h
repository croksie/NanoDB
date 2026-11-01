#pragma once
#include <vector>
#include "Token.h"
#include "AST.h"




class Parser {

private:
	std::vector<Token> m_tokens;
	std::vector<ASTNode> m_nodes;

	size_t m_current = 0;

public:
	Parser(std::vector<Token> tokens);
	virtual ~Parser() = default;

	Token peek();
	Token advance();
	bool check(TokenType type);
	Token consume(TokenType type, std::string error_msg);

	std::unique_ptr<Statement> parseStatement();
	std::unique_ptr<SelectStatement> parseSelect();
	std::unique_ptr<DropTableStatement> parseDropTable();
	std::unique_ptr<CreateTableStatement> parseCreateTable();
	std::unique_ptr<InsertStatement> parseInsert();
	std::unique_ptr<TableRef>   parseTableRef();
	std::unique_ptr<Expression> parseExpression();
	std::unique_ptr<Expression> parseOr();
	std::unique_ptr<Expression> parseAnd();
	std::unique_ptr<Expression> parseComparison();
	std::unique_ptr<Expression> parsePrimary();
};