#pragma once

#include "storage/Log.h"

#include <memory>
#include <string>
#include <vector>

class Partition {

private:

    int id;

    std::unique_ptr<Log> log;

public:

    Partition(
        int id,
        const std::string& dataDirectory
    );

    uint64_t append(
        const std::string& key,
        const std::string& value
    );

    std::vector<Message> readFrom(
        uint64_t offset,
        size_t maxMessages
    );

    uint64_t getNextOffset() const;

    int getId() const;
};