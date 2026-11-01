#include "Parser.h"
#include "DatabaseException.h"

Parser::Parser(std::vector<Token> tokens) : m_tokens(tokens){}

Token Parser::peek()
{
	return m_tokens.at(m_current);
}

Token Parser::advance()
{
	Token token = this->peek();
	m_current++;
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
	m_current++;
	return token;
}

std::unique_ptr<Statement> Parser::parseStatement()
{
	switch (this->peek().type) {
		case TokenType::KEYWORD_SELECT:
			return this->parseSelect();
			break;
		case TokenType::KEYWORD_DROP:
			return this->parseDropTable();
		case TokenType::KEYWORD_CREATE:
			return this->parseCreateTable();
		case TokenType::KEYWORD_INSERT:
			return this->parseInsert();
		default:
			throw DatabaseException("Expression non reconnue");
			break;
	}
	return nullptr;
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
	this->consume(TokenType::KEYWORD_TABLE, "TABLE expected after CREATE");
	Token table = this->consume(TokenType::IDENTIFIER, "Table name expected after TABLE");
	if (this->check(TokenType::SEMICOLON)) {
		this->advance();
	}
	auto result = std::make_unique<DropTableStatement>();
	result->tableName = table.value;
	return result;
}

std::unique_ptr<CreateTableStatement> Parser::parseCreateTable()
{
	this->advance();
	this->consume(TokenType::KEYWORD_TABLE, "TABLE expected after CREATE");
	Token table = this->consume(TokenType::IDENTIFIER, "Table name expected after TABLE");
	this->consume(TokenType::LEFT_PARENTHESIS, "Parenthesis expected");

	auto result = std::make_unique<CreateTableStatement>();
	auto columns = std::vector<ColumnDefinition>();

	while (!this->check(TokenType::RIGHT_PARENTHESIS)) {
		Token columnName = this->consume(TokenType::IDENTIFIER, "Column Name expected");
		if (this->check(TokenType::KEYWORD_NUMBER)) {
			this->advance();
			columns.emplace_back(columnName.value, Type::Number, 0);
		}
		else if (this->check(TokenType::KEYWORD_VARCHAR)) {
			this->advance();
			int size = 255;
			if (this->check(TokenType::LEFT_PARENTHESIS)) {
				this->advance();
				Token sizeToken = this->consume(TokenType::NUMBER, "Expected a number");
				size = std::stoi(sizeToken.value);
				this->consume(TokenType::RIGHT_PARENTHESIS, "Expected right parenthesis");
			}
			columns.emplace_back(columnName.value, Type::Varchar, size);
		}
		else {
			throw DatabaseException("Invalid Token for column type");
		}

		if (this->check(TokenType::COMMA)) {
			this->advance();
		}
		else {
			break;
		}
	}
	this->consume(TokenType::RIGHT_PARENTHESIS, "Expected right parenthesis");
	if (this->check(TokenType::SEMICOLON)) {
		this->advance();
	}
	result->tableName = table.value;
	result->columns = columns;
	return result;
}

std::unique_ptr<InsertStatement> Parser::parseInsert()
{
	this->advance();
	this->consume(TokenType::KEYWORD_INTO, "INTO expected after INSERT");
	Token table = this->consume(TokenType::IDENTIFIER, "Table name expected after INTO");
	auto columnsNames = std::vector<std::string>();
	if (this->check(TokenType::LEFT_PARENTHESIS)) {
		this->advance();
		while (!this->check(TokenType::RIGHT_PARENTHESIS)) {
			Token columnName = this->consume(TokenType::IDENTIFIER, "Column Name expected");
			columnsNames.emplace_back(columnName.value);
			if (this->check(TokenType::COMMA)) {
				this->advance();
			}
			else {
				break;
			}
		}
		this->consume(TokenType::RIGHT_PARENTHESIS, "Expected right parenthesis after column list");
	}

	this->consume(TokenType::KEYWORD_VALUES, "Expected Token VALUES");
	this->consume(TokenType::LEFT_PARENTHESIS, "Expected left parenthesis");

	auto columnsValue = std::vector<std::unique_ptr<Expression>>();
	while (!this->check(TokenType::RIGHT_PARENTHESIS)) {
		columnsValue.emplace_back(this->parsePrimary());
		if (this->check(TokenType::COMMA)) {
			this->advance();
		}
		else {
			break;
		}
	}
	this->consume(TokenType::RIGHT_PARENTHESIS, "Expected right parenthesis after values");
	if (this->check(TokenType::SEMICOLON)) {
		this->advance();
	}

	auto result = std::make_unique<InsertStatement>();
	result->tableName = table.value;
	result->columns = columnsNames;
	result->valuesGrid = std::move(columnsValue);
	return result;
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
		orExpr.get()->op = BinaryOpType::LOGICAL_OR;
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

