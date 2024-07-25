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

void socket_binding(sockaddr_in *server_addr, int *server_socket)
{
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);

    if (bind(*server_socket, (struct sockaddr*)server_addr, sizeof(*server_addr)) == -1)
    {
        close(*server_socket);
        std::string error_msg = "bind failed: " + std::string(strerror(errno));
        throw std::runtime_error(error_msg);
    }
}

void start_listening(int *server_socket)
{
    const int BACKLOG = 10; // Maximum length of the queue of pending connections

    if (listen(*server_socket, BACKLOG) == -1) {
        close(*server_socket);
        std::string error_msg = "listen failed: " + std::string(strerror(errno));
        throw std::runtime_error(error_msg);
    }
}