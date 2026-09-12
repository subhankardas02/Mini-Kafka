#include "common/Utils.h"

#include <chrono>
#include <sstream>

namespace Utils {

uint64_t currentTimeMillis() {

    auto now = std::chrono::system_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        );

    return duration.count();
}

std::vector<std::string> split(
    const std::string& str,
    char delimiter
) {

    std::vector<std::string> result;

    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

std::string trim(const std::string& str) {

    size_t start = str.find_first_not_of(" \t\r\n");

    if (start == std::string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(" \t\r\n");

    return str.substr(
        start,
        end - start + 1
    );
}

}