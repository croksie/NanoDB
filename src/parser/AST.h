#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "DataType.h"
#include "DatabaseException.h"

/* BASE NODE */

struct ASTNode {
    virtual ~ASTNode() = default;
};


/* EXPRESSION */

struct Expression : public ASTNode {};

enum class LiteralType { INT, FLOAT, STRING, BOOLEAN, NULL_VAL };

struct LiteralExpr : public Expression {
    LiteralType type;
    std::string rawValue;
};

struct ColumnRefExpr : public Expression {
    std::string tableName;
    std::string columnName;
};

enum class BinaryOpType {
    EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    LOGICAL_AND, LOGICAL_OR,
    ADD, SUBTRACT, MULTIPLY, DIVIDE
};

inline BinaryOpType matchOpType(std::string val) {
    static const std::unordered_map<std::string, BinaryOpType> type = {
        {"=", BinaryOpType::EQUAL},
        {"!=", BinaryOpType::NOT_EQUAL},
        {"<", BinaryOpType::LESS},
        {"<=", BinaryOpType::LESS_EQUAL},
        {">", BinaryOpType::GREATER},
        {">=", BinaryOpType::GREATER_EQUAL},
        {"and", BinaryOpType::LOGICAL_AND},
        {"or", BinaryOpType::LOGICAL_OR},
        {"+", BinaryOpType::ADD},
        {"-", BinaryOpType::SUBTRACT},
        {"*", BinaryOpType::MULTIPLY},
        {"/", BinaryOpType::DIVIDE}
    };
    auto it = type.find(val);
    return (it != type.end()) ? it->second : throw DatabaseException("Erreur de type lors du parsing");
}

struct BinaryOpExpr : public Expression {
    std::unique_ptr<Expression> left;
    BinaryOpType op;
    std::unique_ptr<Expression> right;
};

/* TABLE REFERENCE*/

struct TableRef {};

struct BaseTableRef : public TableRef {
    std::string tableName;
};

// TODO : Add join


/* STATEMENT */

struct Statement : public ASTNode {};

// --- SELECT ---

struct SelectStatement : public Statement {
    std::vector<std::unique_ptr<Expression>> projections;
    std::unique_ptr<TableRef> fromTable;
    std::unique_ptr<Expression> whereClause;
};


// --- CREATE TABLE ---

struct ColumnDefinition {
    std::string name;
    Type type;
    int size = 0;

    bool isNullable = true;
    // TODO : Add others constraints
};

struct CreateTableStatement : public Statement {
    std::string tableName;
    std::vector<ColumnDefinition> columns;
};

// --- DROP TABLE ---
struct DropTableStatement : public Statement {
    std::string tableName;
};

// --- INSERT ---
struct InsertStatement : public Statement {
    std::string tableName;
    std::vector<std::string> columns;

    std::vector<std::vector<std::unique_ptr<Expression>>> valuesGrid;
};
