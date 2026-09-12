#include "broker/Broker.h"

#include "broker/ClientHandler.h"
#include "common/Logger.h"
#include "network/Socket.h"

#include <thread>

Broker::Broker(
    int brokerPort,
    const std::string& dataDirectory
)
    : port(brokerPort),
      serverFd(-1),
      topicManager(dataDirectory) {
}

Broker::~Broker() {

    if (serverFd >= 0) {
        Socket::closeSocket(serverFd);
    }
}

void Broker::start() {

    serverFd =
        Socket::createServer(port);

    Logger::info(
        "Mini Kafka broker started"
    );

    Logger::info(
        "Listening on port " +
        std::to_string(port)
    );

    while (true) {

        int clientFd =
            Socket::acceptClient(serverFd);

        if (clientFd < 0) {
            continue;
        }

        std::thread(
            ClientHandler::handle,
            clientFd,
            std::ref(topicManager)
        ).detach();
    }
}

TopicManager& Broker::getTopicManager() {

    return topicManager;
}