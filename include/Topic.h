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
    void produce(const std::string& message);
    std::optional<std::string> consume(size_t offset, std::chrono::milliseconds timeout);

private:
    std::vector<std::string> messages_;
    std::mutex mtx_;
    std::condition_variable cv_;
};

}