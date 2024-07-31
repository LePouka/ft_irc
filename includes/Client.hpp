#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <map>

#define MAX_EVENTS 10

class Client {
public:
    //CONST/DEST
    Client(int socket);
    Client();
    ~Client();

    //GETTER/SETTER
    int getSocket() const;
    void setNick(const std::string& nick);
    void setUser(const std::string& user);
    std::string getNick() const;
    std::string getUser() const;

private:
    int socket;
    std::string nick;
    std::string user;
};

class ClientArray {
    private:
        std::map<int, Client> clients;
    public:
        //CONST/DEST
        ClientArray();
        ~ClientArray();

        //GETTER/SETTER
        Client &getClient(int clientSocket);
        void addClient(int clientSocket, Client client);
        
        //METHODS
        void removeClient(int clientSocket);
};