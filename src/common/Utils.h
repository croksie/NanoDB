#pragma once
#include <vector>

template <typename... Args>
auto row(Args&&... args) {
    std::vector<std::unique_ptr<DataValue>> r;
    (r.push_back(std::move(args)), ...);
    return r;
}