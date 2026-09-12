#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Utils {

uint64_t currentTimeMillis();

std::vector<std::string> split(
    const std::string& str,
    char delimiter
);

std::string trim(const std::string& str);

}