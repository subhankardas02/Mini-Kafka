#include "Topic.h"

namespace minikafka {

void Topic::produce(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx_);
    messages_.push_back(message);
    cv_.notify_all(); // Wake up any waiting consumers
}

std::optional<std::string> Topic::consume(size_t offset, std::chrono::milliseconds timeout) {
    std::unique_lock<std::mutex> lock(mtx_);
    
    // Wait until the offset is valid (message exists) or timeout occurs
    bool message_available = cv_.wait_for(lock, timeout, [this, offset] { 
        return offset < messages_.size(); 
    });

    if (message_available) {
        return messages_[offset];
    }
    return std::nullopt; // Timeout reached, no new message
}

} // namespace minikafka