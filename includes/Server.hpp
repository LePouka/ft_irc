#pragma once

#include <map>
#include <string>
#include <sys/epoll.h>
#include "Client.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>

class Server {
public:
    Server(int port);
    ~Server();
    void run();

private:
    int server_socket;
    int epoll_fd;
    std::map<int, Client> clients;
    epoll_event ev;
    epoll_event events[10];

    void setNonBlocking(int sock);
    void createSocket();
    void bindSocket(int port);
    void startListening();
    void createEpollInstance();
    void addSocketToEpoll(int sock);
    void eventLoop();
    void handleClientMessage(int client_socket, const std::string& message);
    void sendWelcomeMessage(int client_socket);
    void sendErrorMessage(int client_socket, const std::string& command);
};
