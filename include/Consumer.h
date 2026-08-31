#pragma once
#include "Broker.h"
#include <string>
#include <chrono>
#include <optional>

namespace minikafka {

class Consumer {
public:
    Consumer(Broker& broker, const std::string& topic_name);
    
    // Poll for the next message. Bumps the internal offset if successful.
    std::optional<std::string> poll(std::chrono::milliseconds timeout = std::chrono::milliseconds(100));

private:
    Broker& broker_;
    std::string topic_name_;
    size_t current_offset_; // The consumer tracks its own place in the log
};

} // namespace minikafka