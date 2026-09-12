#include "storage/Log.h"

#include "common/Logger.h"
#include "common/Utils.h"

#include <filesystem>

Log::Log(const std::string& path)
    : filePath(path),
      nextOffset(0) {

    std::filesystem::create_directories(
        std::filesystem::path(filePath).parent_path()
    );

    file.open(
        filePath,
        std::ios::in |
        std::ios::out |
        std::ios::binary |
        std::ios::app
    );

    if (!file.is_open()) {
        throw std::runtime_error(
            "Failed to open log file: " + filePath
        );
    }

    loadExistingLog();
}

Log::~Log() {

    std::lock_guard<std::mutex> lock(mutex);

    if (file.is_open()) {
        file.close();
    }
}

void Log::loadExistingLog() {

    std::lock_guard<std::mutex> lock(mutex);

    file.seekg(0, std::ios::beg);

    while (true) {

        uint64_t offset;
        uint64_t timestamp;

        uint32_t keySize;
        uint32_t valueSize;

        if (!file.read(
                reinterpret_cast<char*>(&offset),
                sizeof(offset))) {
            break;
        }

        if (!file.read(
                reinterpret_cast<char*>(&timestamp),
                sizeof(timestamp))) {
            break;
        }

        if (!file.read(
                reinterpret_cast<char*>(&keySize),
                sizeof(keySize))) {
            break;
        }

        if (!file.read(
                reinterpret_cast<char*>(&valueSize),
                sizeof(valueSize))) {
            break;
        }

        file.seekg(
            keySize + valueSize,
            std::ios::cur
        );

        nextOffset = offset + 1;
    }

    file.clear();
    file.seekp(0, std::ios::end);
}

uint64_t Log::append(
    const std::string& key,
    const std::string& value
) {

    std::lock_guard<std::mutex> lock(mutex);

    uint64_t offset = nextOffset;

    uint64_t timestamp =
        Utils::currentTimeMillis();

    uint32_t keySize =
        static_cast<uint32_t>(key.size());

    uint32_t valueSize =
        static_cast<uint32_t>(value.size());

    file.clear();

    file.seekp(0, std::ios::end);

    file.write(
        reinterpret_cast<const char*>(&offset),
        sizeof(offset)
    );

    file.write(
        reinterpret_cast<const char*>(&timestamp),
        sizeof(timestamp)
    );

    file.write(
        reinterpret_cast<const char*>(&keySize),
        sizeof(keySize)
    );

    file.write(
        reinterpret_cast<const char*>(&valueSize),
        sizeof(valueSize)
    );

    file.write(
        key.data(),
        key.size()
    );

    file.write(
        value.data(),
        value.size()
    );

    file.flush();

    if (!file.good()) {
        throw std::runtime_error(
            "Failed to write message"
        );
    }

    nextOffset++;

    return offset;
}

std::vector<Message> Log::readFrom(
    uint64_t offset,
    size_t maxMessages
) {

    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Message> messages;

    file.clear();
    file.seekg(0, std::ios::beg);

    while (messages.size() < maxMessages) {

        uint64_t currentOffset;
        uint64_t timestamp;

        uint32_t keySize;
        uint32_t valueSize;

        if (!file.read(
                reinterpret_cast<char*>(&currentOffset),
                sizeof(currentOffset))) {
            break;
        }

        if (!file.read(
                reinterpret_cast<char*>(&timestamp),
                sizeof(timestamp))) {
            break;
        }

        if (!file.read(
                reinterpret_cast<char*>(&keySize),
                sizeof(keySize))) {
            break;
        }

        if (!file.read(
                reinterpret_cast<char*>(&valueSize),
                sizeof(valueSize))) {
            break;
        }

        std::string key(keySize, '\0');
        std::string value(valueSize, '\0');

        if (!file.read(
                key.data(),
                keySize)) {
            break;
        }

        if (!file.read(
                value.data(),
                valueSize)) {
            break;
        }

        if (currentOffset >= offset) {

            messages.emplace_back(
                currentOffset,
                timestamp,
                key,
                value
            );
        }
    }

    file.clear();

    return messages;
}

uint64_t Log::getNextOffset() const {

    std::lock_guard<std::mutex> lock(mutex);

    return nextOffset;
}