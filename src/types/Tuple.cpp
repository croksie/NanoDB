#include "Tuple.h"
#include "DatabaseException.h"


Tuple Tuple::serialize(const std::vector<std::unique_ptr<DataValue>>& values, const std::vector<Column>& columns) {
    std::vector<uint8_t> result;
    for (size_t i = 0; i < columns.size(); i++) {
        Column c = columns.at(i);
        if (c.getDataType()->getTypeName() != values.at(i)->type) {
            throw DatabaseException("Types de donnee incompatibles lors de la serialisation");
        }
        std::vector<uint8_t> tmp = c.getDataType()->serialize(values.at(i)->getValue());
        result.insert(result.end(), tmp.begin(), tmp.end());
    }
    return Tuple(result);
}
std::vector<std::shared_ptr<DataValue>> Tuple::deserialize(const std::vector<Column>& columns) const {
    std::vector<std::shared_ptr<DataValue>> result;
    size_t offset = 0;
    for (size_t i = 0; i < columns.size(); i++) {
        Column c = columns.at(i);
        result.push_back(c.getDataType()->deserialize(this->data, offset));
    }
    return result;
}