#pragma once
#include "AST.h"
#include "Database.h"
#include "Table.h"


class Executor {

private:
	Database& db;

public:
	Executor(Database& db, const SelectStatement& node);
	void executeSelect(const SelectStatement& statement);
};