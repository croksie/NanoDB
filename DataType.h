#pragma once
#include <string>
#include <vector>
#include <any>
#include <memory>
#include "DataValue.h"


class DataType {
private:
	const Type type;
	const bool isFixedSize;
	const int maxSize;

public:
	DataType(Type t, bool fixed, int max)
		: type(t), isFixedSize(fixed), maxSize(max) {}

	virtual ~DataType() = default;

	virtual std::vector<std::byte> serialize(const std::any& value) = 0;
	virtual std::unique_ptr<DataValue> deserialize(const std::vector<std::byte>& data, size_t& offset) = 0;

	const Type getTypeName() const { return type; }
	bool getIsFixedSize() const { return isFixedSize; }
	int getMaxSize() const { return maxSize; }
};

class IntType : public DataType {
public:
	IntType() : DataType(Type::Number, true, sizeof(int)) {}
	std::vector<std::byte> serialize(const std::any& value) override;
	std::unique_ptr<DataValue> deserialize(const std::vector<std::byte>& data, size_t& offset) override;
};

class StringType : public DataType {
public:
	StringType() : DataType(Type::Varchar, true, 255) {}
	std::vector<std::byte> serialize(const std::any& value) override;
	std::unique_ptr<DataValue> deserialize(const std::vector<std::byte>& data, size_t& offset) override;
};