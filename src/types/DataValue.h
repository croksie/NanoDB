#pragma once
#include <string>
#include <any>

enum Type { Number, Varchar};

class DataValue
{
public:
	const Type type;
	
	DataValue(Type t) : type(t) {};
	~DataValue() = default;
	virtual std::any getValue() const = 0;
	virtual std::string toString() const = 0;

};

class IntValue : public DataValue {
public:
	int val;

	IntValue(int v) : DataValue(Type::Number), val(v) {}

	std::any getValue() const { return this->val; }
	std::string toString() const;
};

class StringValue : public DataValue {
public:
	std::string val;

	StringValue(std::string v) : DataValue(Type::Varchar), val(v) {}

	std::any getValue() const { return this->val; }
	std::string toString() const;
};