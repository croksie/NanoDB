#pragma once
#include "AST.h"
#include "Database.h"
#include "Table.h"


class Executor {

private:
	Database db;
	ASTNode maiNnode;

public:
	Executor(const SelectStatement& node);
	void executeSelect(const SelectStatement& statement);
};