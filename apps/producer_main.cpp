#include "client/Producer.h"

#include <iostream>
#include <string>

int main() {

    try {

        Producer producer(
            "127.0.0.1",
            9092
        );

        std::cout
            << "Mini Kafka Producer\n";

        std::cout
            << "Commands:\n"
            << "  create <topic> <partitions>\n"
            << "  produce <topic> <partition> <key> <value>\n"
            << "  quit\n\n";

        std::string command;

        while (true) {

            std::cout << "> ";

            std::cin >> command;

            if (command == "create") {

                std::string topic;
                int partitions;

                std::cin
                    >> topic
                    >> partitions;

                producer.createTopic(
                    topic,
                    partitions
                );
            }

            else if (command == "produce") {

                std::string topic;
                int partition;
                std::string key;
                std::string value;

                std::cin
                    >> topic
                    >> partition
                    >> key;

                std::getline(
                    std::cin,
                    value
                );

                if (!value.empty() &&
                    value[0] == ' ') {

                    value.erase(0, 1);
                }

                producer.produce(
                    topic,
                    partition,
                    key,
                    value
                );
            }

            else if (command == "quit") {

                break;
            }

            else {

                std::cout
                    << "Unknown command\n";
            }
        }

    }
    catch (const std::exception& e) {

        std::cerr
            << "Producer error: "
            << e.what()
            << std::endl;

        return 1;
    }

    return 0;
}