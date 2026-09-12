#include "broker/TopicManager.h"

#include <filesystem>
#include <stdexcept>

Topic::Topic(
    const std::string& topicName,
    int partitionCount,
    const std::string& dataDirectory
)
    : name(topicName) {

    std::string topicDirectory =
        dataDirectory + "/" + topicName;

    std::filesystem::create_directories(
        topicDirectory
    );

    for (int i = 0; i < partitionCount; ++i) {

        partitions.push_back(
            std::make_unique<Partition>(
                i,
                topicDirectory + "/p" +
                std::to_string(i)
            )
        );
    }
}

Partition& Topic::getPartition(
    int partition
) {

    if (
        partition < 0 ||
        partition >= static_cast<int>(partitions.size())
    ) {
        throw std::out_of_range(
            "Invalid partition"
        );
    }

    return *partitions[partition];
}

int Topic::getPartitionCount() const {

    return static_cast<int>(
        partitions.size()
    );
}

const std::string& Topic::getName() const {

    return name;
}

TopicManager::TopicManager(
    const std::string& directory
)
    : dataDirectory(directory) {

    std::filesystem::create_directories(
        dataDirectory
    );
}

void TopicManager::createTopic(
    const std::string& name,
    int partitions
) {

    if (partitions <= 0) {
        throw std::invalid_argument(
            "Partition count must be positive"
        );
    }

    std::lock_guard<std::mutex> lock(mutex);

    if (topics.find(name) != topics.end()) {
        throw std::runtime_error(
            "Topic already exists"
        );
    }

    topics[name] =
        std::make_unique<Topic>(
            name,
            partitions,
            dataDirectory
        );
}

Topic* TopicManager::getTopic(
    const std::string& name
) {

    std::lock_guard<std::mutex> lock(mutex);

    auto it = topics.find(name);

    if (it == topics.end()) {
        return nullptr;
    }

    return it->second.get();
}

std::vector<std::string>
TopicManager::listTopics() {

    std::lock_guard<std::mutex> lock(mutex);

    std::vector<std::string> result;

    for (const auto& [name, topic] : topics) {
        result.push_back(name);
    }

    return result;
}