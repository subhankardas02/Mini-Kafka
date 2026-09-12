#include "network/Protocol.h"

namespace Protocol {

std::string ok(
    const std::string& message
) {

    return "OK " + message + "\n";
}

std::string error(
    const std::string& message
) {

    return "ERROR " + message + "\n";
}

std::string message(
    uint64_t offset,
    const std::string& key,
    const std::string& value
) {

    return "MESSAGE " +
           std::to_string(offset) +
           " " +
           key +
           " " +
           value +
           "\n";
}

}