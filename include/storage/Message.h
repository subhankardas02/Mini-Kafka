#pragma once

#include <cstdint>
#include <string>

struct Message {
    uint64_t offset;
    uint64_t timestamp;
    std::string key;
    std::string value;

    Message();

    Message(
        uint64_t offset,
        uint64_t timestamp,
        const std::string& key,
        const std::string& value
    );
};