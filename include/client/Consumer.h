#pragma once

#include "storage/Message.h"

#include <cstdint>
#include <string>
#include <vector>

class Consumer {

private:

    int socketFd;

public:

    Consumer(
        const std::string& host,
        int port
    );

    ~Consumer();

    std::vector<Message> fetch(
        const std::string& topic,
        int partition,
        uint64_t offset,
        size_t maxMessages
    );
};