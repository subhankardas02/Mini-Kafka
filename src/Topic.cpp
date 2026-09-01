#include "Topic.h"

namespace minikafka {

void Topic::produce(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx_);
    messages_.push_back(message);
    cv_.notify_all();
}

std::optional<std::string> Topic::consume(size_t offset, std::chrono::milliseconds timeout) {
    std::unique_lock<std::mutex> lock(mtx_);
    
    bool message_available = cv_.wait_for(lock, timeout, [this, offset] { 
        return offset < messages_.size(); 
    });

    if (message_available) {
        return messages_[offset];
    }
    return std::nullopt;
}

}