#include "network/Socket.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace Socket {

int createServer(int port) {

    int serverFd =
        socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

    if (serverFd < 0) {
        throw std::runtime_error(
            "socket() failed"
        );
    }

    int opt = 1;

    setsockopt(
        serverFd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    );

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (
        bind(
            serverFd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        ) < 0
    ) {

        close(serverFd);

        throw std::runtime_error(
            "bind() failed"
        );
    }

    if (listen(serverFd, 100) < 0) {

        close(serverFd);

        throw std::runtime_error(
            "listen() failed"
        );
    }

    return serverFd;
}

int acceptClient(int serverFd) {

    sockaddr_in clientAddress{};
    socklen_t clientLength =
        sizeof(clientAddress);

    return accept(
        serverFd,
        reinterpret_cast<sockaddr*>(&clientAddress),
        &clientLength
    );
}

int connectToServer(
    const std::string& host,
    int port
) {

    int fd =
        socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

    if (fd < 0) {
        throw std::runtime_error(
            "socket() failed"
        );
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (
        inet_pton(
            AF_INET,
            host.c_str(),
            &address.sin_addr
        ) <= 0
    ) {

        close(fd);

        throw std::runtime_error(
            "Invalid server address"
        );
    }

    if (
        connect(
            fd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        ) < 0
    ) {

        close(fd);

        throw std::runtime_error(
            "connect() failed"
        );
    }

    return fd;
}

bool sendAll(
    int fd,
    const std::string& data
) {

    size_t sent = 0;

    while (sent < data.size()) {

        ssize_t n =
            send(
                fd,
                data.data() + sent,
                data.size() - sent,
                0
            );

        if (n <= 0) {
            return false;
        }

        sent += n;
    }

    return true;
}

bool receiveLine(
    int fd,
    std::string& line
) {

    line.clear();

    char c;

    while (true) {

        ssize_t n =
            recv(
                fd,
                &c,
                1,
                0
            );

        if (n == 0) {
            return false;
        }

        if (n < 0) {

            if (errno == EINTR) {
                continue;
            }

            return false;
        }

        if (c == '\n') {
            return true;
        }

        line.push_back(c);

        if (line.size() > 1024 * 1024) {
            return false;
        }
    }
}

void closeSocket(int fd) {

    if (fd >= 0) {
        close(fd);
    }
}

}