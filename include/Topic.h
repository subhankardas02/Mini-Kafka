#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <chrono>

namespace minikafka {

class Topic {
public:
    // Append a message to the log
    void produce(const std::string& message);

    // Consume a message at a specific offset. Blocks until available or timeout.
    std::optional<std::string> consume(size_t offset, std::chrono::milliseconds timeout);

private:
    std::vector<std::string> messages_; // The append-only log
    std::mutex mtx_;
    std::condition_variable cv_;
};

} // namespace minikafka