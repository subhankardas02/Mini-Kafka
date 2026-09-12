#pragma once

#include <string>

class Producer {

private:

    int socketFd;

public:

    Producer(
        const std::string& host,
        int port
    );

    ~Producer();

    bool createTopic(
        const std::string& topic,
        int partitions
    );

    bool produce(
        const std::string& topic,
        int partition,
        const std::string& key,
        const std::string& value
    );
};