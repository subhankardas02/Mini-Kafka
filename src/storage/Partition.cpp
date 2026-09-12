#include "storage/Partition.h"

#include <filesystem>

Partition::Partition(
    int partitionId,
    const std::string& dataDirectory
)
    : id(partitionId) {

    std::filesystem::create_directories(
        dataDirectory
    );

    std::string path =
        dataDirectory +
        "/partition-" +
        std::to_string(id) +
        ".log";

    log = std::make_unique<Log>(path);
}

uint64_t Partition::append(
    const std::string& key,
    const std::string& value
) {

    return log->append(
        key,
        value
    );
}

std::vector<Message> Partition::readFrom(
    uint64_t offset,
    size_t maxMessages
) {

    return log->readFrom(
        offset,
        maxMessages
    );
}

uint64_t Partition::getNextOffset() const {

    return log->getNextOffset();
}

int Partition::getId() const {

    return id;
}