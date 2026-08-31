cmake_minimum_required(VERSION 3.10)
project(MiniKafkaTCP)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost and Threads
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

include_directories(include)

# 1. The Broker Server
add_executable(kafka_server 
    src/Topic.cpp 
    src/Broker.cpp 
    src/server.cpp
)
target_link_libraries(kafka_server Threads::Threads Boost::boost)

# 2. The Producer Client
add_executable(kafka_producer src/producer.cpp)
target_link_libraries(kafka_producer Threads::Threads Boost::boost)

# 3. The Consumer Client
add_executable(kafka_consumer src/consumer.cpp)
target_link_libraries(kafka_consumer Threads::Threads Boost::boost)