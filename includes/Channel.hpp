#pragma once

#include <set>
#include <string>

class Channel {
public:
    Channel(const std::string& name);
    void addUser(int socket);
    void removeUser(int socket);
    void setTopic(const std::string& topic);
    bool isOperator(int socket) const; 
    void setPassword(const std::string& password);
    void setMode(const std::string& mode);
    void setLimit(int limit);
    void addOperator(int socket);
    void removeOperator(int socket);
    void broadcastMessage(const std::string& message, int exceptSocket = -1);

private:
    std::string name;
    std::string topic;
    std::string password;
    std::string mode;
    int limit;
    std::set<int> users;
    std::set<int> operators;
};
