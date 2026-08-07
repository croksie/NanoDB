#include "Parser.h"
#include "DatabaseException.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens){}

Token Parser::peek()
{
	return this->tokens.at(this->current);
}

Token Parser::advance()
{
	Token token = this->peek();
	this->current++;
	return token;
}

bool Parser::check(TokenType type)
{
	return (this->peek().type == type);
}

Token Parser::consume(TokenType type, std::string error_msg)
{
	Token token = this->peek();
	if (token.type != type) {
		throw DatabaseException(error_msg);
	}
	this->current++;
	return token;
}

std::unique_ptr<ASTNode> Parser::parseStatement()
{
	switch (this->peek().type) {
		case TokenType::KEYWORD_SELECT:
			return this->parseSelect();
			break;
		case TokenType::KEYWORD_DROP:
			return this->parseDropTable();
		default:
			throw DatabaseException("Expression non reconnue");
			break;
	}
	return std::unique_ptr<ASTNode>();
}

std::unique_ptr<SelectStatement> Parser::parseSelect()
{
	std::unique_ptr<SelectStatement> result = std::make_unique<SelectStatement>();
	this->advance();

	std::vector<std::unique_ptr<Expression>> projections;
	projections.push_back(this->parsePrimary());
	while (this->check(TokenType::COMMA)) {
		this->advance();
		projections.push_back(this->parsePrimary());
	}
	result->projections = std::move(projections);
	this->consume(TokenType::KEYWORD_FROM, "Besoin d'un from aprés la liste des projection");
	result->fromTable = this->parseTableRef();
	if(this->check(TokenType::KEYWORD_WHERE)) {
		this->advance();
		result->whereClause = this->parseExpression();
	}

	this->consume(TokenType::SEMICOLON, "Il manque le point virgule à la fin de la requête");


	return std::move(result);
}

std::unique_ptr<DropTableStatement> Parser::parseDropTable()
{
	this->advance();
	this->consume(TokenType::KEYWORD_TABLE, "Table attendue aprés DROP");
	this->advance();
	Token table = this->consume(TokenType::KEYWORD_TABLE, "Nom de la table attendue aprés DROP");
	auto result = std::make_unique<DropTableStatement>();
	result->tableName = table.value;
	return std::move(result);
}

std::unique_ptr<TableRef> Parser::parseTableRef()
{
	std::unique_ptr<BaseTableRef> result = std::make_unique<BaseTableRef>();
	result->tableName = this->consume(TokenType::IDENTIFIER, "Le nom de la table est incorrect").value;
	return std::move(result);
}

std::unique_ptr<Expression> Parser::parseExpression()
{
	auto result = this->parseOr();
	return result;
}



std::unique_ptr<Expression> Parser::parseOr()
{
	auto left = this->parseAnd();
	while (this->peek().type == TokenType::OR) {
		this->advance();
		auto orExpr = std::make_unique<BinaryOpExpr>();
		orExpr.get()->left = std::move(left);
		orExpr.get()->op = BinaryOpType::LOGICAL_AND;
		orExpr.get()->right = this->parseAnd();
		left = std::move(orExpr);
	}
	return left;
}

std::unique_ptr<Expression> Parser::parseAnd()
{
	auto left = this->parseComparison();
	while (this->peek().type == TokenType::AND) {
		this->advance();
		auto andExpr = std::make_unique<BinaryOpExpr>();
		andExpr.get()->left = std::move(left);
		andExpr.get()->op = BinaryOpType::LOGICAL_AND;
		andExpr.get()->right = this->parseComparison();
		left = std::move(andExpr);
	}
	return left;
}

std::unique_ptr<Expression> Parser::parseComparison()
{

	auto binExpr = std::make_unique<BinaryOpExpr>();
	binExpr.get()->left = this->parsePrimary();
	binExpr.get()->op = matchOpType(this->peek().value);
	this->advance();
	binExpr.get()->right = this->parsePrimary();
	return binExpr;
}

std::unique_ptr<Expression> Parser::parsePrimary()
{
	Token token = this->peek();

	if (token.type == TokenType::IDENTIFIER) {
		this->advance();
		auto colRef = std::make_unique<ColumnRefExpr>();
		colRef->columnName = token.value;
		return colRef;
	}

	if (token.type == TokenType::NUMBER) {
		this->advance();
		auto lit = std::make_unique<LiteralExpr>();
		lit->type = LiteralType::INT;
		lit->rawValue = token.value;
		return lit;
	}

	if (token.type == TokenType::VARCHAR) {
		this->advance();
		auto lit = std::make_unique<LiteralExpr>();
		lit->type = LiteralType::STRING;
		lit->rawValue = token.value;
		return lit;
	}

	if (token.type == TokenType::STAR) {
		this->advance();
		auto colRef = std::make_unique<ColumnRefExpr>();
		colRef->columnName = "*";
		return colRef;
	}

	throw DatabaseException("Expression invalide : '" + token.value + "'");
}

