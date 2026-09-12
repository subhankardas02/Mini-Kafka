#include "broker/ClientHandler.h"

#include "common/Logger.h"
#include "common/Utils.h"
#include "network/Protocol.h"
#include "network/Socket.h"

#include <sstream>

void ClientHandler::handle(
    int clientFd,
    TopicManager& topicManager
) {

    Logger::info(
        "Client connected"
    );

    std::string line;

    while (
        Socket::receiveLine(
            clientFd,
            line
        )
    ) {

        line = Utils::trim(line);

        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);

        std::string command;

        ss >> command;

        try {

            if (command == "PING") {

                Socket::sendAll(
                    clientFd,
                    Protocol::ok("PONG")
                );
            }

            else if (command == "CREATE_TOPIC") {

                std::string topic;
                int partitions;

                ss >> topic >> partitions;

                if (topic.empty()) {

                    Socket::sendAll(
                        clientFd,
                        Protocol::error(
                            "Invalid topic"
                        )
                    );

                    continue;
                }

                topicManager.createTopic(
                    topic,
                    partitions
                );

                Socket::sendAll(
                    clientFd,
                    Protocol::ok(
                        "TOPIC_CREATED"
                    )
                );
            }

            else if (command == "PRODUCE") {

                std::string topic;
                int partition;
                std::string key;

                ss >> topic;
                ss >> partition;
                ss >> key;

                std::string value;

                std::getline(
                    ss,
                    value
                );

                value =
                    Utils::trim(value);

                Topic* t =
                    topicManager.getTopic(topic);

                if (t == nullptr) {

                    Socket::sendAll(
                        clientFd,
                        Protocol::error(
                            "Topic does not exist"
                        )
                    );

                    continue;
                }

                Partition& p =
                    t->getPartition(partition);

                uint64_t offset =
                    p.append(
                        key,
                        value
                    );

                Socket::sendAll(
                    clientFd,
                    Protocol::ok(
                        "OFFSET " +
                        std::to_string(offset)
                    )
                );
            }

            else if (command == "FETCH") {

                std::string topic;
                int partition;

                uint64_t offset;
                size_t maxMessages;

                ss >> topic;
                ss >> partition;
                ss >> offset;
                ss >> maxMessages;

                Topic* t =
                    topicManager.getTopic(topic);

                if (t == nullptr) {

                    Socket::sendAll(
                        clientFd,
                        Protocol::error(
                            "Topic does not exist"
                        )
                    );

                    continue;
                }

                Partition& p =
                    t->getPartition(partition);

                auto messages =
                    p.readFrom(
                        offset,
                        maxMessages
                    );

                Socket::sendAll(
                    clientFd,
                    Protocol::ok(
                        "MESSAGES " +
                        std::to_string(
                            messages.size()
                        )
                    )
                );

                for (const auto& msg : messages) {

                    Socket::sendAll(
                        clientFd,
                        Protocol::message(
                            msg.offset,
                            msg.key,
                            msg.value
                        )
                    );
                }
            }

            else if (command == "LIST_TOPICS") {

                auto topics =
                    topicManager.listTopics();

                Socket::sendAll(
                    clientFd,
                    Protocol::ok(
                        "TOPICS " +
                        std::to_string(
                            topics.size()
                        )
                    )
                );

                for (const auto& topic : topics) {

                    Socket::sendAll(
                        clientFd,
                        topic + "\n"
                    );
                }
            }

            else if (command == "QUIT") {

                Socket::sendAll(
                    clientFd,
                    Protocol::ok("BYE")
                );

                break;
            }

            else {

                Socket::sendAll(
                    clientFd,
                    Protocol::error(
                        "Unknown command"
                    )
                );
            }

        }
        catch (const std::exception& e) {

            Socket::sendAll(
                clientFd,
                Protocol::error(
                    e.what()
                )
            );
        }
    }

    Socket::closeSocket(clientFd);

    Logger::info(
        "Client disconnected"
    );
}