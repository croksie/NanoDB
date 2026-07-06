#include "DataType.h"



 /* ******************* Int ******************* */

//
std::vector<std::byte> IntType::serialize(const std::any& value)
{
    int v = std::any_cast<int>(value);
    std::vector<std::byte> result(sizeof(int));

    for (size_t i = 0; i < sizeof(int); ++i)
        result[i] = static_cast<std::byte>((v >> (8 * i)) & 0xFF);

    return result;
}

std::any IntType::deserialize(const std::vector<std::byte>& data)
{
    int value = 0;
    for (size_t i = 0; i < sizeof(int) && i < data.size(); ++i)
        value |= static_cast<int>(std::to_integer<unsigned char>(data[i])) << (8 * i);

    return value;
}


/* ******************* String ******************* */

//
std::vector<std::byte> StringType::serialize(const std::any& value)
{
    const std::string& str = std::any_cast<const std::string&>(value);

    std::vector<std::byte> result;
    result.reserve(sizeof(size_t) + str.size());

    size_t length = str.size();
    for (size_t i = 0; i < sizeof(size_t); ++i)
    {
        result.push_back(static_cast<std::byte>((length >> (8 * i)) & 0xFF));
    }

    for (char c : str)
    {
        result.push_back(static_cast<std::byte>(c));
    }

    return result;
}


std::any StringType::deserialize(const std::vector<std::byte>& data)
{
    if (data.size() < sizeof(size_t))
    {
        return std::string();
    }

    size_t length = 0;
    for (size_t i = 0; i < sizeof(size_t); ++i)
    {
        length |= static_cast<size_t>(std::to_integer<unsigned char>(data[i])) << (8 * i);
    }

    length = std::min(length, data.size() - sizeof(size_t));

    std::string value;
    value.reserve(length);
    for (size_t i = 0; i < length; ++i)
    {
        value.push_back(static_cast<char>(data[sizeof(size_t) + i]));
    }

    return value;
}