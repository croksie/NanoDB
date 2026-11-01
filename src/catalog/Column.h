#pragma once
#include "DataType.h"
#include <memory>
#include <string>


class Column {
  std::string m_name;
  std::shared_ptr<DataType> m_dataType;

  // Constraints

  bool m_isNullable = false;

public:
  Column(std::shared_ptr<DataType> dataType, std::string name)
      : m_dataType(dataType), m_name(name) {}
  Column(std::vector<uint8_t> data);

  ~Column() = default;

  std::shared_ptr<DataType> getDataType() const { return m_dataType; }
  std::string getName() const { return m_name; }
  bool getIsNullable() const { return m_isNullable; }

  std::vector<uint8_t> serialize();
};
