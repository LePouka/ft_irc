#include "../includes/irc.h"


int main() {
    try {
        int server_socket = 0;
        int epoll_fd = 0;
        struct sockaddr_in server_addr;
        struct epoll_event ev, events[MAX_EVENTS];

        socket_creation(&server_socket);
        // Bind the socket to an address and port
        socket_binding(&server_addr, &server_socket);

        // Start listening for connections
        start_listening(&server_socket);

        // Create an epoll instance
        instance_creation(&epoll_fd, &server_socket);

        // Add the server socket to the epoll instance
        socket_to_instance(&server_socket, &epoll_fd, &ev);

        std::cout << "IRC server started on port " << PORT << "\n";

        // Event loop
        event_loop(&server_socket, &epoll_fd, events, &ev);

        close(server_socket);
        close(epoll_fd);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}