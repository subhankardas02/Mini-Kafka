#include "client/Producer.h"

#include "network/Socket.h"

#include <iostream>

Producer::Producer(
    const std::string& host,
    int port
) {

    socketFd =
        Socket::connectToServer(
            host,
            port
        );
}

Producer::~Producer() {

    Socket::closeSocket(
        socketFd
    );
}

bool Producer::createTopic(
    const std::string& topic,
    int partitions
) {

    std::string request =
        "CREATE_TOPIC " +
        topic +
        " " +
        std::to_string(partitions) +
        "\n";

    if (
        !Socket::sendAll(
            socketFd,
            request
        )
    ) {
        return false;
    }

    std::string response;

    if (
        !Socket::receiveLine(
            socketFd,
            response
        )
    ) {
        return false;
    }

    std::cout
        << response
        << std::endl;

    return true;
}

bool Producer::produce(
    const std::string& topic,
    int partition,
    const std::string& key,
    const std::string& value
) {

    std::string request =
        "PRODUCE " +
        topic +
        " " +
        std::to_string(partition) +
        " " +
        key +
        " " +
        value +
        "\n";

    if (
        !Socket::sendAll(
            socketFd,
            request
        )
    ) {
        return false;
    }

    std::string response;

    if (
        !Socket::receiveLine(
            socketFd,
            response
        )
    ) {
        return false;
    }

    std::cout
        << response
        << std::endl;

    return true;
}