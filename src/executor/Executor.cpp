#include "Executor.h"

Executor::Executor(const SelectStatement& node)
{
	this->executeSelect(node);
}

void Executor::executeSelect(const SelectStatement& statement){
	auto* fromTable = static_cast<BaseTableRef*>(statement.fromTable.get());
	Table* table = this->db.getTable(fromTable->tableName);
	auto* proj = statement.projections.at(0).get();

	ColumnRefExpr* cre = static_cast<ColumnRefExpr*>(proj);
	std::vector<Tuple> tuples;
	if (statement.whereClause == nullptr) {
		tuples = table->getTuples();
	}
	auto columns = table->getColumns();
	if (cre->columnName == "*") {
		for (Tuple tuple : tuples) {
			size_t offset = 0;
			for (size_t colIdx = 0; colIdx < columns.size(); colIdx++) {
				Column c = columns.at(colIdx);
				std::unique_ptr<DataValue> valuePtr = c.getDataType()->deserialize(tuple.data, offset);
				std::cout << valuePtr->toString() << " ";
			}
			std::cout << std::endl;
		}
	}
}
