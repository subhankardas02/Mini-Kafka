#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9000));
        std::cout << "Connected to Broker at 127.0.0.1:9000\n";

        std::string topic = "system-logs";
        size_t current_offset = 0;

        while (true) {
            // Format: FETCH <topic> <offset>\n
            std::string request = "FETCH " + topic + " " + std::to_string(current_offset) + "\n";
            boost::asio::write(socket, boost::asio::buffer(request));

            boost::asio::streambuf buffer;
            boost::asio::read_until(socket, buffer, '\n');
            
            std::istream is(&buffer);
            std::string response_type, payload;
            is >> response_type;
            std::getline(is, payload);

            if (response_type == "MSG") {
                // Clean leading space
                if (!payload.empty() && payload[0] == ' ') payload = payload.substr(1);
                if (!payload.empty() && payload.back() == '\r') payload.pop_back();

                std::cout << "[Consumer] Processed (Offset " << current_offset << "): " << payload << "\n";
                current_offset++; // Advance offset only on success
            } else if (response_type == "NONE") {
                // Timeout occurred, just loop and poll again
                std::cout << "[Consumer] Polling... no new messages.\n";
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Consumer Error: " << e.what() << "\n";
    }
    return 0;
}