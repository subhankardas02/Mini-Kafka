#pragma once

#include "storage/Message.h"

#include <cstdint>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

class Log {

private:

    std::string filePath;

    std::fstream file;

    uint64_t nextOffset;

    mutable std::mutex mutex;

    void loadExistingLog();

public:

    explicit Log(const std::string& filePath);

    ~Log();

    uint64_t append(
        const std::string& key,
        const std::string& value
    );

    std::vector<Message> readFrom(
        uint64_t offset,
        size_t maxMessages
    );

    uint64_t getNextOffset() const;

};