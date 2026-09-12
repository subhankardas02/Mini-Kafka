#include "client/Consumer.h"

#include <iostream>

int main() {

    try {

        Consumer consumer(
            "127.0.0.1",
            9092
        );

        std::string topic;

        int partition;

        uint64_t offset;

        size_t maxMessages;

        std::cout
            << "Mini Kafka Consumer\n\n";

        std::cout
            << "Topic: ";

        std::cin >> topic;

        std::cout
            << "Partition: ";

        std::cin >> partition;

        std::cout
            << "Starting offset: ";

        std::cin >> offset;

        std::cout
            << "Maximum messages: ";

        std::cin >> maxMessages;

        auto messages =
            consumer.fetch(
                topic,
                partition,
                offset,
                maxMessages
            );

        std::cout
            << "\nReceived "
            << messages.size()
            << " messages\n\n";

        for (const auto& message : messages) {

            std::cout
                << "Offset: "
                << message.offset
                << "\n";

            std::cout
                << "Key: "
                << message.key
                << "\n";

            std::cout
                << "Value: "
                << message.value
                << "\n";

            std::cout
                << "--------------------\n";
        }

    }
    catch (const std::exception& e) {

        std::cerr
            << "Consumer error: "
            << e.what()
            << std::endl;

        return 1;
    }

    return 0;
}