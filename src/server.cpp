#include "Broker.h"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <sstream>

using boost::asio::ip::tcp;
using namespace minikafka;

void handle_client(tcp::socket socket, Broker& broker) {
    try {
        boost::asio::streambuf buffer;
        while (true) {
            // Read from socket until a newline is found
            boost::system::error_code error;
            boost::asio::read_until(socket, buffer, '\n', error);
            
            if (error == boost::asio::error::eof) break; // Client disconnected
            else if (error) throw boost::system::system_error(error);

            // Parse the incoming string
            std::istream is(&buffer);
            std::string command, topic_name, payload;
            is >> command >> topic_name;
            std::getline(is, payload);
            
            // Clean up the leading space from getline and trailing carriage returns
            if (!payload.empty() && payload[0] == ' ') payload = payload.substr(1);
            if (!payload.empty() && payload.back() == '\r') payload.pop_back();

            // Route to the Broker
            if (command == "PRODUCE") {
                broker.get_or_create_topic(topic_name)->produce(payload);
                boost::asio::write(socket, boost::asio::buffer("OK\n"));
                std::cout << "[Server] Produced to " << topic_name << ": " << payload << "\n";
            } 
            else if (command == "FETCH") {
                size_t offset = std::stoull(payload);
                // Block for up to 2 seconds waiting for a message
                auto msg = broker.get_or_create_topic(topic_name)->consume(offset, std::chrono::seconds(2));
                
                if (msg) {
                    boost::asio::write(socket, boost::asio::buffer("MSG " + *msg + "\n"));
                } else {
                    boost::asio::write(socket, boost::asio::buffer("NONE\n"));
                }
            }
        }
    } catch (std::exception& e) {
        std::cerr << "[Server] Client error: " << e.what() << "\n";
    }
}

int main() {
    Broker broker;
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 9000));

    std::cout << "Mini Kafka Broker listening on port 9000...\n";

    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "[Server] New client connected.\n";
        
        // Spawn a thread for each connection (Thread-per-connection model)
        std::thread(handle_client, std::move(socket), std::ref(broker)).detach();
    }
    return 0;
}