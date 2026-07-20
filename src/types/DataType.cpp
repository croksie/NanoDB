#include "DataType.h"


 /* ******************* Int ******************* */

//
std::vector<uint8_t> IntType::serialize(const std::any& value)
{
    int v = std::any_cast<int>(value);
    std::vector<uint8_t> result(sizeof(int));

    for (size_t i = 0; i < sizeof(int); ++i)
        result[i] = static_cast<uint8_t>((v >> (8 * i)) & 0xFF);

    return result;
}


std::unique_ptr<DataValue> IntType::deserialize(const std::vector<uint8_t>& data, size_t& offset)
{
    int value = 0;
    for (size_t i = 0; i < sizeof(int) && i < data.size(); ++i)
        value |= static_cast<int>(data[i + offset]) << (8 * i);
    offset += sizeof(int);
    return std::make_unique<IntValue>(value);
}


/* ******************* String ******************* */

//
std::vector<uint8_t> StringType::serialize(const std::any& value)
{
    const std::string& str = std::any_cast<const std::string&>(value);

    std::vector<uint8_t> result;
    result.reserve(sizeof(size_t) + str.size());

    size_t length = str.size();
    for (size_t i = 0; i < sizeof(size_t); ++i)
    {
        result.push_back(static_cast<uint8_t>((length >> (8 * i)) & 0xFF));
    }

    for (char c : str)
    {
        result.push_back(static_cast<uint8_t>(c));
    }

    return result;
}


std::unique_ptr<DataValue> StringType::deserialize(const std::vector<uint8_t>& data, size_t& offset)
{
    if (data.size() < sizeof(size_t))
    {
        return std::make_unique<StringValue>(std::string());
    }

    size_t length = 0;
    for (size_t i = 0; i < sizeof(size_t); ++i)
    {
        length |= static_cast<size_t>(data[i + offset]) << (8 * i);
    }

    length = std::min(length, data.size() - sizeof(size_t));

    std::string value;
    value.reserve(length);
    for (size_t i = 0; i < length; ++i)
    {
        value.push_back(static_cast<char>(data[sizeof(size_t) + i + offset]));
    }
    offset += sizeof(size_t) + length;
    return std::make_unique<StringValue>(value);
}