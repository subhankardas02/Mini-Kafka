#include "broker/Broker.h"
#include "common/Logger.h"

#include <exception>

int main() {

    try {

        Broker broker(
            9092,
            "./data"
        );

        broker.start();

    }
    catch (const std::exception& e) {

        Logger::error(
            e.what()
        );

        return 1;
    }

    return 0;
}