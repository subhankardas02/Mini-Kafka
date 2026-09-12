#pragma once

#include <cstdint>
#include <string>

namespace Protocol {

std::string ok(
    const std::string& message
);

std::string error(
    const std::string& message
);

std::string message(
    uint64_t offset,
    const std::string& key,
    const std::string& value
);

}