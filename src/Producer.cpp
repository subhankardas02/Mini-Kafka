#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9000));
        std::cout << "Connected to Broker at 127.0.0.1:9000\n";

        for (int i = 1; i <= 5; ++i) {
            std::string topic = "system-logs";
            std::string msg = "Log_Entry_" + std::to_string(i);
            
            std::string request = "PRODUCE " + topic + " " + msg + "\n";
            boost::asio::write(socket, boost::asio::buffer(request));

            boost::asio::streambuf buffer;
            boost::asio::read_until(socket, buffer, '\n');

            std::cout << "[Producer] Sent: " << msg << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    } catch (std::exception& e) {
        std::cerr << "Producer Error: " << e.what() << "\n";
    }
    return 0;
}