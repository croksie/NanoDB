#pragma once
#include <string>
#include <vector>
#include <any>
#include <memory>
#include <cstdint>
#include "DataValue.h"


class DataType {
private:
	const Type m_type;
	const bool m_isFixedSize;
	const int m_maxSize;

public:
	DataType(Type t, bool fixed, int max)
		: m_type(t), m_isFixedSize(fixed), m_maxSize(max) {}

	virtual ~DataType() = default;

	virtual std::vector<uint8_t> serialize(const std::any& value) = 0;
	virtual std::unique_ptr<DataValue> deserialize(const std::vector<uint8_t>& data, size_t& offset) = 0;

	const Type getTypeName() const { return m_type; }
	bool getIsFixedSize() const { return m_isFixedSize; }
	int getMaxSize() const { return m_maxSize; }
};

class IntType : public DataType {
public:
	IntType() : DataType(Type::Number, true, sizeof(int)) {}
	std::vector<uint8_t> serialize(const std::any& value) override;
	std::unique_ptr<DataValue> deserialize(const std::vector<uint8_t>& data, size_t& offset) override;
};

class StringType : public DataType {
public:
	StringType() : DataType(Type::Varchar, true, 255) {}
	std::vector<uint8_t> serialize(const std::any& value) override;
	std::unique_ptr<DataValue> deserialize(const std::vector<uint8_t>& data, size_t& offset) override;
};