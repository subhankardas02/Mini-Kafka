#pragma once

#include "broker/TopicManager.h"

class Broker {

private:

    int port;

    int serverFd;

    TopicManager topicManager;

public:

    Broker(
        int port,
        const std::string& dataDirectory
    );

    ~Broker();

    void start();

    TopicManager& getTopicManager();

private:

    void handleClient(int clientFd);
};