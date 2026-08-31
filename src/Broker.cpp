#include "Broker.h"

namespace minikafka {

std::shared_ptr<Topic> Broker::get_or_create_topic(const std::string& topic_name) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (topics_.find(topic_name) == topics_.end()) {
        topics_[topic_name] = std::make_shared<Topic>();
    }
    return topics_[topic_name];
}

} // namespace minikafka