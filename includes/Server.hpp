#pragma once

#include <map>
#include <string>
#include <sys/epoll.h>
#include "Client.hpp"
#include "Channel.hpp"

#define MAX_EVENTS 10

class Server {
public:
    Server(int port, const std::string& password);
    ~Server();
    void run();

private:
    int server_socket;
    int epoll_fd;
    std::map<int, Client> clients;
    std::map<std::string, Channel> channels;
    std::string password;

    void setNonBlocking(int sock);
    void createSocket();
    void bindSocket(int port);
    void startListening();
    void createEpollInstance();
    void addSocketToEpoll(int sock);
    void eventLoop();
    // Add this declaration in Server.hpp
    bool checkAuthentication(int client_socket);
    void handleUnknownCommand(int client_socket);
    void handleClientMessage(int client_socket, const std::string& message);

    // Command handlers
    void handleCAP(int client_socket, const std::string& args);
    void handlePASS(int client_socket, const std::string& args);
    void handleNICK(int client_socket, const std::string& args);
    void handleUSER(int client_socket, const std::string& args);
    void handleJOIN(int client_socket, const std::string& args);
    void handlePART(int client_socket, const std::string& args);
    void handlePRIVMSG(int client_socket, const std::string& args);
    void handleKICK(int client_socket, const std::string& args);
    void handleINVITE(int client_socket, const std::string& args);
    void handleTOPIC(int client_socket, const std::string& args);
    void handleMODE(int client_socket, const std::string& args);
};
