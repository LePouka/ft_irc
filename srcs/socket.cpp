#include "../includes/irc.h"

void set_nonblocking(int sock)
{
    int opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        throw std::runtime_error("fcntl(F_GETFL) failed");
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0) {
        throw std::runtime_error("fcntl(F_SETFL) failed");
    }
}

void socket_creation(int *server_socket)
{
        // Create a socket
        *server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (*server_socket == -1) {
            throw std::runtime_error("socket creation failed");
        }

        // Make the server socket non-blocking
        set_nonblocking(*server_socket);
}

