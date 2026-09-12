cmake_minimum_required(VERSION 3.16)

project(MiniKafka
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

include_directories(include)

set(COMMON_SRC
    src/common/Logger.cpp
    src/common/Utils.cpp
)

set(NETWORK_SRC
    src/network/Socket.cpp
    src/network/Protocol.cpp
)

set(STORAGE_SRC
    src/storage/Message.cpp
    src/storage/Partition.cpp
    src/storage/Log.cpp
)

set(BROKER_SRC
    src/broker/Broker.cpp
    src/broker/ClientHandler.cpp
    src/broker/TopicManager.cpp
)

set(CLIENT_SRC
    src/client/Producer.cpp
    src/client/Consumer.cpp
    src/storage/Message.cpp
)

add_executable(mini-kafka-broker
    apps/broker_main.cpp
    ${COMMON_SRC}
    ${NETWORK_SRC}
    ${STORAGE_SRC}
    ${BROKER_SRC}
)

add_executable(mini-kafka-producer
    apps/producer_main.cpp
    ${COMMON_SRC}
    ${NETWORK_SRC}
    ${CLIENT_SRC}
)

add_executable(mini-kafka-consumer
    apps/consumer_main.cpp
    ${COMMON_SRC}
    ${NETWORK_SRC}
    ${CLIENT_SRC}
)

target_link_libraries(mini-kafka-broker pthread)
target_link_libraries(mini-kafka-producer pthread)
target_link_libraries(mini-kafka-consumer pthread)

