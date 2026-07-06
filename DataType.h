#pragma once
#include <string>
#include <vector>


class DataType {

private:
	const std::string typeName;
	const bool isFixedSize;
	const int maxSize;

public:
	//virtual std::vector<std::byte> serialize() = 0;  // Type de retour à revoir
	//a faire la suite

	DataType(std::string typeName, bool fixed, int max) : typeName(typeName), isFixedSize(fixed), maxSize(max) {}
};

class IntType : public DataType {
public:
	IntType() : DataType("Int", true, sizeof(int)) {}
};

class StringType : public DataType {
public:
	StringType() : DataType("String", true, 255) {}
};