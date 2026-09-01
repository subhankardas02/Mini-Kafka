#pragma once
#include "Topic.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

namespace minikafka {

class Broker {
public:
    std::shared_ptr<Topic> get_or_create_topic(const std::string& topic_name);

private:
    std::unordered_map<std::string, std::shared_ptr<Topic>> topics_;
    std::mutex mtx_;
};

}