#include "DataValue.h"

std::string IntValue::toString() const
{
    return std::to_string(this->val);
}

std::string StringValue::toString() const
{
    return this->val;
}
