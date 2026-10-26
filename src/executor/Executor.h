#pragma once
#include "AST.h"
#include "Database.h"
#include "Table.h"


class Executor {

private:
	Database& db;

	std::vector<size_t> resolveProjections(
		const std::vector<std::unique_ptr<Expression>>& projections,
		const std::vector<Column>& tableColumns
	);
	bool evaluateWhere(
		const Expression* expr,
		const std::vector<std::shared_ptr<DataValue>>& row,
		const std::vector<Column>& tableColumns
	);

	std::shared_ptr<DataValue> evaluateExpr(
		const Expression* expr,
		const std::vector<std::shared_ptr<DataValue>>& row,
		const std::vector<Column>& tableColumns
	);

	bool compareValues(
		const DataValue& left,
		BinaryOpType op,
		const DataValue& right
	);

public:
	Executor(Database& db, const SelectStatement& node);
	void executeSelect(const SelectStatement& statement);
};