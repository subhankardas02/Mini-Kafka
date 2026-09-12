#include "client/Consumer.h"

#include "network/Socket.h"
#include "common/Utils.h"

#include <iostream>
#include <sstream>

Consumer::Consumer(
    const std::string& host,
    int port
) {

    socketFd =
        Socket::connectToServer(
            host,
            port
        );
}

Consumer::~Consumer() {

    Socket::closeSocket(
        socketFd
    );
}

std::vector<Message> Consumer::fetch(
    const std::string& topic,
    int partition,
    uint64_t offset,
    size_t maxMessages
) {

    std::string request =
        "FETCH " +
        topic +
        " " +
        std::to_string(partition) +
        " " +
        std::to_string(offset) +
        " " +
        std::to_string(maxMessages) +
        "\n";

    Socket::sendAll(
        socketFd,
        request
    );

    std::string response;

    Socket::receiveLine(
        socketFd,
        response
    );

    std::stringstream header(
        response
    );

    std::string status;
    std::string messagesWord;
    size_t count;

    header
        >> status
        >> messagesWord
        >> count;

    std::vector<Message> messages;

    if (status != "OK") {

        std::cerr
            << response
            << std::endl;

        return messages;
    }

    for (size_t i = 0; i < count; ++i) {

        std::string line;

        if (
            !Socket::receiveLine(
                socketFd,
                line
            )
        ) {
            break;
        }

        std::stringstream ss(line);

        std::string messageWord;

        uint64_t msgOffset;

        std::string key;

        ss
            >> messageWord
            >> msgOffset
            >> key;

        std::string value;

        std::getline(
            ss,
            value
        );

        value =
            Utils::trim(value);

        messages.emplace_back(
            msgOffset,
            0,
            key,
            value
        );
    }

    return messages;
}