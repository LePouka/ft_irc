#pragma once

#include <string>

class Client {
public:
    Client(int socket = -1);
    int getSocket() const;
    void setNick(const std::string& nick);
    void setUser(const std::string& user);
    std::string getNick() const;
    std::string getUser() const;
    void setAuthenticated(bool auth);

private:
    int socket;
    std::string nick;
    std::string user;
    bool authenticated;
};
