#include "DataValue.h"

std::string IntValue::toString() const
{
    return std::to_string(m_val);
}

std::string StringValue::toString() const
{
    return m_val;
}
