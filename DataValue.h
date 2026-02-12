#pragma once
#include <string>
#include <list>

//Abstract DataValueClass
class DataValue
{
public:
    virtual void display() = 0;
};

//String Value
class StringValue : public DataValue
{
public:
    StringValue(const std::string& Value);
    void display() override;

private:
    std::string Value;
};

//Int Value
class IntValue : public DataValue
{
public:
    IntValue(const int& Value);
    void display() override;

private:
    int Value;
};

//List Value
class ListValue : public DataValue
{
public:
    ListValue(const std::list<std::string>& Value);
    void display() override;

private:
    std::list<std::string> Value;
};