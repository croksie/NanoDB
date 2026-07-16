#include "Column.h"

std::vector<uint8_t> Column::serialize()
{
	std::vector<uint8_t> result;

	// Serialize name
	size_t nameLength = name.size();
	result.reserve(sizeof(size_t) + nameLength + 2); // size + name bytes + 1 byte for type + 1 byte for nullable

	// Serialize name length
	for (size_t i = 0; i < sizeof(size_t); ++i) {
		result.push_back(static_cast<uint8_t>((nameLength >> (8 * i)) & 0xFF));
	}

	// Serialize name characters
	for (char c : name) {
		result.push_back(static_cast<uint8_t>(c));
	}

	// Serialize Type
	uint8_t typeVal = static_cast<uint8_t>(data->getTypeName());
	result.push_back(typeVal);

	// Serialize isNullable
	uint8_t nullableVal = isNullable ? 1 : 0;
	result.push_back(nullableVal);

	return result;
}

Column::Column(std::vector<uint8_t> data)
{
	size_t offset = 0;

	// Deserialize name length
	if (data.size() < sizeof(size_t)) {
		name = "";
		this->data = std::make_shared<IntType>();
		isNullable = false;
		return;
	}

	size_t nameLength = 0;
	for (size_t i = 0; i < sizeof(size_t); ++i) {
		nameLength |= static_cast<size_t>(data[offset++]) << (8 * i);
	}

	// Deserialize name
	if (offset + nameLength > data.size()) {
		name = "";
		this->data = std::make_shared<IntType>();
		isNullable = false;
		return;
	}

	name.reserve(nameLength);
	for (size_t i = 0; i < nameLength; ++i) {
		name.push_back(static_cast<char>(data[offset++]));
	}

	// Deserialize Type
	if (offset >= data.size()) {
		this->data = std::make_shared<IntType>();
		isNullable = false;
		return;
	}
	uint8_t typeVal = data[offset++];
	if (typeVal == Type::Number) {
		this->data = std::make_shared<IntType>();
	} else {
		this->data = std::make_shared<StringType>();
	}

	// Deserialize isNullable
	if (offset < data.size()) {
		isNullable = (data[offset++] != 0);
	} else {
		isNullable = false;
	}
}
