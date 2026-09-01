#pragma once
#include "Broker.h"
#include <string>

namespace minikafka {

class Producer {
public:
    explicit Producer(Broker& broker);
    void send(const std::string& topic_name, const std::string& message);

private:
    Broker& broker_;
};

}