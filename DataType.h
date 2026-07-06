#pragma once
#include <string>
#include <vector>
#include <any>


class DataType {

private:
	const std::string typeName;
	const bool isFixedSize;
	const int maxSize;

public:
	DataType(std::string typeName, bool fixed, int max)
		: typeName(std::move(typeName)), isFixedSize(fixed), maxSize(max) {}

	virtual ~DataType() = default;

	virtual std::vector<std::byte> serialize(const std::any& value) = 0;
	virtual std::any deserialize(const std::vector<std::byte>& data) = 0;

	const std::string& getTypeName() const { return typeName; }
	bool getIsFixedSize() const { return isFixedSize; }
	int getMaxSize() const { return maxSize; }
};

class IntType : public DataType {
public:
	IntType() : DataType("Int", true, sizeof(int)) {}
	std::vector<std::byte> serialize(const std::any& value) override;
	std::any deserialize(const std::vector<std::byte>& data) override;
};

class StringType : public DataType {
public:
	StringType() : DataType("String", true, 255) {}
	std::vector<std::byte> serialize(const std::any& value) override;
	std::any deserialize(const std::vector<std::byte>& data) override;
};