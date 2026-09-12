#pragma once

#include "storage/Partition.h"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class Topic {

private:

    std::string name;

    std::vector<std::unique_ptr<Partition>> partitions;

public:

    Topic(
        const std::string& name,
        int partitionCount,
        const std::string& dataDirectory
    );

    Partition& getPartition(int partition);

    int getPartitionCount() const;

    const std::string& getName() const;
};

class TopicManager {

private:

    std::string dataDirectory;

    std::unordered_map<
        std::string,
        std::unique_ptr<Topic>
    > topics;

    mutable std::mutex mutex;

public:

    explicit TopicManager(
        const std::string& dataDirectory
    );

    void createTopic(
        const std::string& name,
        int partitions
    );

    Topic* getTopic(
        const std::string& name
    );

    std::vector<std::string> listTopics();

};