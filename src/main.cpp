#include "Broker.h"
#include "Producer.h"
#include "Consumer.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace minikafka;

int main() {
    Broker broker;

    std::thread consumer_thread([&broker]() {
        Consumer consumer(broker, "user-events");
        std::cout << "[Consumer] Started listening to 'user-events'...\n";
        
        while(true) {
            auto msg = consumer.poll(std::chrono::milliseconds(500));
            if (msg) {
                std::cout << "[Consumer] Processed: " << *msg << "\n";
                if (*msg == "EOF") break; 
            }
        }
    });

    Producer producer(broker);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    for(int i = 1; i <= 5; ++i) {
        std::string payload = "Event_Payload_" + std::to_string(i);
        std::cout << "[Producer] Sending: " << payload << "\n";
        
        producer.send("user-events", payload);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    producer.send("user-events", "EOF");

    consumer_thread.join();

    std::cout << "Mini-Kafka demo completed.\n";
    return 0;
    // j
}