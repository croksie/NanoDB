#include "Executor.h"
#include <algorithm>

std::vector<size_t> Executor::resolveProjections(const std::vector<std::unique_ptr<Expression>>& projections, const std::vector<Column>& tableColumns)
{
	auto indexes = std::vector<size_t>();
	auto* cre = dynamic_cast<const ColumnRefExpr*>(projections.at(0).get());
	if (cre == nullptr) {
		throw DatabaseException("Bad expression");
	}
	else {
		for (auto& expr : projections) {
			cre = dynamic_cast<const ColumnRefExpr*>(expr.get());
			if (cre == nullptr) {
				throw DatabaseException("Bad expression");
			}
			else {
				auto it = std::find_if(tableColumns.begin(), tableColumns.end(),
					[&](const Column& colRef) {
						return cre->columnName == colRef.getName();
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
			if (tableColumns[i].getName() == colRef->columnName)
				return row[i];
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

void Executor::executeSelect(const SelectStatement& statement){
	auto* fromTable = static_cast<BaseTableRef*>(statement.fromTable.get());
	Table* table = this->db.getTable(fromTable->tableName);
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
