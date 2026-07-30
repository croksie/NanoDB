#pragma once
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <memory>
#include "DataValue.h"

template <typename... Args>
auto row(Args&&... args) {
    std::vector<std::unique_ptr<DataValue>> r;
    (r.push_back(std::move(args)), ...);
    return r;
}

inline std::string lowerStr(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}