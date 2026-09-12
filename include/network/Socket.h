#pragma once

#include <string>

namespace Socket {

int createServer(int port);

int acceptClient(int serverFd);

int connectToServer(
    const std::string& host,
    int port
);

bool sendAll(
    int fd,
    const std::string& data
);

bool receiveLine(
    int fd,
    std::string& line
);

void closeSocket(int fd);

}