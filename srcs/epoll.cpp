#include "../includes/irc.h"

void instance_creation(int *epoll_fd, int *server_socket)
{
    *epoll_fd = epoll_create1(0);
    if (*epoll_fd == -1) {
        close(*server_socket);
        throw std::runtime_error("epoll_create1 failed");
    }
}

void socket_to_instance(int *server_socket, int *epoll_fd, epoll_event *ev)
{
    ev->events = EPOLLIN;
    ev->data.fd = *server_socket;
    if (epoll_ctl(*epoll_fd, EPOLL_CTL_ADD, *server_socket, ev) == -1) {
        close(*server_socket);
        close(*epoll_fd);
        throw std::runtime_error("epoll_ctl: server_socket failed");
    }  
}

void event_loop(int *server_socket, int *epoll_fd, epoll_event *events, epoll_event *ev)
{
    std::map<int, std::string> client_nicks;
    std::map<int, std::string> client_users;
    while (true) 
    {
        int nfds = epoll_wait(*epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) 
        {
            close(*server_socket);
            close(*epoll_fd);
            throw std::runtime_error("epoll_wait failed");
        }

        for (int n = 0; n < nfds; ++n) 
        {
            if (events[n].data.fd == *server_socket)
            {
                // Handle new connections
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_socket = accept(*server_socket, (struct sockaddr*)&client_addr, &client_len);
                if (client_socket == -1)
                {
                    std::cerr << "accept failed\n";
                    continue;
                }
                set_nonblocking(client_socket);

                ev->events = EPOLLIN | EPOLLET;
                ev->data.fd = client_socket;
                if (epoll_ctl(*epoll_fd, EPOLL_CTL_ADD, client_socket, ev) == -1)
                {
                    close(client_socket);
                    std::cerr << "epoll_ctl: client_socket failed\n";
                    continue;
                }
                std::cout << "New client connected.\n";
            }
            else 
            {
                // Handle data from a connected client
                char buffer[1024];
                int client_socket = events[n].data.fd;
                int bytes_read = read(client_socket, buffer, sizeof(buffer));
                if (bytes_read == -1)
                {
                    std::cerr << "read failed\n";
                    close(client_socket);
                    continue;
                } 
                else if (bytes_read == 0)
                {
                    // Client disconnected
                    std::cout << "Client disconnected.\n";
                    close(client_socket);
                    client_nicks.erase(client_socket);
                    client_users.erase(client_socket);
                }
                else
                {
                    // Null-terminate and process the message
                    buffer[bytes_read] = '\0';
                    std::string message(buffer);
                    handle_client_message(client_socket, message, &client_nicks, &client_users);
                }
            }
        }
    }
}