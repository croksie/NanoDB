#pragma once
#include <string>
#include <any>

enum Type { Number, Varchar};

class DataValue
{
public:
	const Type m_type;
	
	DataValue(Type t) : m_type(t) {};
	virtual ~DataValue() = default;
	virtual std::any getValue() const = 0;
	virtual std::string toString() const = 0;

};

class IntValue : public DataValue {
public:
	int m_val;

	IntValue(int v) : DataValue(Type::Number), m_val(v) {}

	std::any getValue() const { return m_val; }
	std::string toString() const;
};

class StringValue : public DataValue {
public:
	std::string m_val;

	StringValue(std::string v) : DataValue(Type::Varchar), m_val(v) {}

	std::any getValue() const { return m_val; }
	std::string toString() const;
};