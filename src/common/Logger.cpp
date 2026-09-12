#include "common/Logger.h"

#include <iostream>
#include <mutex>

namespace {

std::mutex logMutex;

}

namespace Logger {

void info(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    std::cout << "[INFO] "
              << message
              << std::endl;
}

void error(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    std::cerr << "[ERROR] "
              << message
              << std::endl;
}

}