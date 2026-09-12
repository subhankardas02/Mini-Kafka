#pragma once

#include "broker/TopicManager.h"

class ClientHandler {

public:

    static void handle(
        int clientFd,
        TopicManager& topicManager
    );

};