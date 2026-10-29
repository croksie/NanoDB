#include "Executor.h"
#include "Lexer.h"
#include "Parser.h"
#include "Utils.h"
#include <algorithm>

std::vector<size_t> Executor::resolveProjections(const std::vector<std::unique_ptr<Expression>>& projections, const std::vector<Column>& tableColumns)
{
	auto indexes = std::vector<size_t>();
	auto* cre = dynamic_cast<const ColumnRefExpr*>(projections.at(0).get());
	if (cre == nullptr) {
		throw DatabaseException("Bad expression");
	}
	else {
		if (cre->columnName == "*") {
			for (size_t i = 0; i < tableColumns.size(); ++i) {
				indexes.push_back(i);
			}
			return indexes;
		}
		for (auto& expr : projections) {
			cre = dynamic_cast<const ColumnRefExpr*>(expr.get());
			if (cre == nullptr) {
				throw DatabaseException("Bad expression");
			}
			else {
				auto it = std::find_if(tableColumns.begin(), tableColumns.end(),
					[&](const Column& colRef) {
						return lowerStr(cre->columnName) == lowerStr(colRef.getName());
					});
				if (it == tableColumns.end()) {
					throw DatabaseException("Column not found");
				}
				indexes.push_back(std::distance(tableColumns.begin(), it));
			}
		}
	}
	return indexes;
}

bool Executor::evaluateWhere(const Expression* expr, const std::vector<std::shared_ptr<DataValue>>& row, const std::vector<Column>& tableColumns)
{
	if (expr == nullptr) {
		return true;
	}
	const auto* bin = dynamic_cast<const BinaryOpExpr*>(expr);
	if (bin == nullptr) {
		throw DatabaseException("Bad expression");
	} else {
		switch (bin->op)
		{
		case BinaryOpType::LOGICAL_AND:
			return evaluateWhere(bin->left.get(), row, tableColumns) && evaluateWhere(bin->right.get(), row, tableColumns);
			break;
		case BinaryOpType::LOGICAL_OR:
			return evaluateWhere(bin->left.get(), row, tableColumns) || evaluateWhere(bin->right.get(), row, tableColumns);
			break;
		default:
			auto leftValue = evaluateExpr(bin->left.get(), row, tableColumns);
			auto rightValue = evaluateExpr(bin->right.get(), row, tableColumns);
			return compareValues(*leftValue, bin->op, *rightValue);
			break;
		}
	}
	return false;
}

std::shared_ptr<DataValue> Executor::evaluateExpr(const Expression* expr,
	const std::vector<std::shared_ptr<DataValue>>& row,
	const std::vector<Column>& tableColumns)
{
	if (const auto* lit = dynamic_cast<const LiteralExpr*>(expr)) {
		switch (lit->type) {
		case LiteralType::INT:
			return std::make_shared<IntValue>(std::stoi(lit->rawValue));
		case LiteralType::STRING:
			return std::make_shared<StringValue>(lit->rawValue);

		default:
			return nullptr;
		}
		return nullptr;
	}

	if (const auto* colRef = dynamic_cast<const ColumnRefExpr*>(expr)) {
		for (size_t i = 0; i < tableColumns.size(); ++i) {
			if (lowerStr(tableColumns[i].getName()) == lowerStr(colRef->columnName))
				return row[i];
		}
		return nullptr;
	}

	return nullptr;
}

std::unique_ptr<DataValue> Executor::evaluateExpr(const Expression* expr)
{
	if (const auto* lit = dynamic_cast<const LiteralExpr*>(expr)) {
		switch (lit->type) {
		case LiteralType::INT:
			return std::make_unique<IntValue>(std::stoi(lit->rawValue));
		case LiteralType::STRING:
			return std::make_unique<StringValue>(lit->rawValue);

		default:
			return nullptr;
		}
		return nullptr;
	}
	return nullptr;
}

bool Executor::compareValues(const DataValue& left, BinaryOpType op, const DataValue& right)
{
	if (left.type != right.type) {
		throw DatabaseException("Compare two different type is not allowed");
	}
	switch (op)
	{
	case BinaryOpType::EQUAL:
		if (left.toString() == right.toString()) {
			return true;
		}
		break;
	case BinaryOpType::NOT_EQUAL:
		if (left.toString() != right.toString()) {
			return true;
		}
		break;
	case BinaryOpType::LESS:
		if (left.toString() < right.toString()) {
			return true;
		}
		break;
	case BinaryOpType::LESS_EQUAL:
		if (left.toString() <= right.toString()) {
			return true;
		}
		break;
	case BinaryOpType::GREATER:
		if (left.toString() > right.toString()) {
			return true;
		}
		break;
	case BinaryOpType::GREATER_EQUAL:
		if (left.toString() >= right.toString()) {
			return true;
		}
		break;
	default:
		throw DatabaseException("Operator not allowed");
		break;
	}

	return false;
}

Executor::Executor(Database& db, const SelectStatement& node) : db(db)
{
	this->executeSelect(node);
}

Executor::Executor(Database& db, std::string input) : db(db)
{
	Lexer lexer = Lexer(input);
	std::vector<Token> tokens = lexer.Analyse();
	Parser parser = Parser(tokens);
	auto stmt = parser.parseStatement();
	if (stmt) {
		this->executeStatement(*stmt);
	}
}

void Executor::executeStatement(const Statement& statement) {
	switch (statement.statementType) {
	case StatementType::SELECT:
		this->executeSelect(static_cast<const SelectStatement&>(statement));
		break;
	case StatementType::DROP_TABLE:
		this->executeDrop(static_cast<const DropTableStatement&>(statement));
		break;
	case StatementType::CREATE_TABLE:
		this->executeCreate(static_cast<const CreateTableStatement&>(statement));
		break;
	case StatementType::INSERT:
		this->executeInsert(static_cast<const InsertStatement&>(statement));
		break;
	}
}

void Executor::executeSelect(const SelectStatement& statement){
	auto* fromTable = static_cast<BaseTableRef*>(statement.fromTable.get());
	Table* table = this->db.getTable(fromTable->tableName);
	if (!table) {
		throw DatabaseException("Table '" + fromTable->tableName + "' not found");
	}
	auto& columns = table->getColumns();
	auto proj = resolveProjections(statement.projections, columns);

	std::vector<Tuple> tuples = table->getTuples();

	std::cout << "Resultat de la requete : " << std::endl;
	for (Tuple tuple : tuples) {
		size_t offset = 0;
		auto row = tuple.deserialize(columns);
		bool pass = evaluateWhere(statement.whereClause.get(), row, columns);
		if (pass){
			for (auto colIdx : proj) {
				std::cout << row.at(colIdx)->toString() << " ";
			}
			std::cout << std::endl;
		}
	}
	
}

void Executor::executeDrop(const DropTableStatement& statement) {
	db.deleteTable(statement.tableName);
	std::cout << "Table '" << statement.tableName << "' delete." << std::endl;
}

void Executor::executeCreate(const CreateTableStatement& statement) {
	auto columns = std::vector<Column>();
	for (auto c : statement.columns){
		switch (c.type) {
		case Type::Number:
			columns.emplace_back(std::make_shared<IntType>(), c.name);
			break;
		case Type::Varchar:
			columns.emplace_back(std::make_shared<StringType>(), c.name);
			break;
		default:
			throw DatabaseException("Type not supported");
			break;
		}
	};

	db.createTable(statement.tableName, columns);
	std::cout << "Table '" << statement.tableName << "' created." << std::endl;
}

void Executor::executeInsert(const InsertStatement& statement)
{
	Table* table = this->db.getTable(statement.tableName);
	if (!table) {
		throw DatabaseException("Table '" + statement.tableName + "' not found");
	}

	const std::vector<Column>& columns = table->getColumns();
	auto values = std::vector<std::unique_ptr<DataValue>>();
	values.resize(columns.size());

	if (statement.columns.empty()) {
		if (statement.valuesGrid.size() != columns.size()) {
			throw DatabaseException("Invalid number of column");
		}
		for (size_t i = 0; i < columns.size(); i++) {
			values[i] = evaluateExpr(statement.valuesGrid.at(i).get());
			if (!values[i]) {
				throw DatabaseException("Invalid value for column " + columns[i].getName());
			}
			if (values[i]->type != columns[i].getDataType()->getTypeName()) {
				throw DatabaseException("DataType not compatible during serialization");
			}
		}
	}
	else {
		if (statement.columns.size() != statement.valuesGrid.size()) {
			throw DatabaseException("Invalid number of column");
		}

		for (const auto& colName : statement.columns) {
			auto it = std::find_if(columns.begin(), columns.end(),
				[&](const Column& col) { return lowerStr(col.getName()) == lowerStr(colName); });
			if (it == columns.end()) {
				throw DatabaseException("Column '" + colName + "' not found in table '" + statement.tableName + "'");
			}
		}

		for (size_t i = 0; i < columns.size(); i++) {
			auto columnName = columns[i].getName();
			auto pos = std::find_if(statement.columns.begin(), statement.columns.end(),
				[&](const std::string& name) { return lowerStr(name) == lowerStr(columnName); });
			if (pos != statement.columns.end()) {
				auto index = std::distance(statement.columns.begin(), pos);
				values[i] = evaluateExpr(statement.valuesGrid.at(index).get());
				if (!values[i]) {
					throw DatabaseException("Invalid value for column " + columnName);
				}
				if (values[i]->type != columns[i].getDataType()->getTypeName()) {
					throw DatabaseException("DataType not compatible during serialization");
				}
			}
			else {
				if (columns[i].getDataType()->getTypeName() == Type::Number) {
					values[i] = std::make_unique<IntValue>(0);
				}
				else {
					values[i] = std::make_unique<StringValue>("");
				}
			}
		}
	}

	table->insertTuple(std::move(values));
	std::cout << "1 ligne inse '" << statement.tableName << "'." << std::endl;
}
