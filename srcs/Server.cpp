#include "../includes/Server.hpp"
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cerrno> 
#include <sstream>

Server::Server(int port, const std::string& password) : password(password) {
    createSocket();
    bindSocket(port);
    startListening();
    createEpollInstance();
}

Server::~Server() {
    close(server_socket);
    close(epoll_fd);
}

void Server::run() {
    eventLoop();
}

void Server::setNonBlocking(int sock) {
    int opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        throw std::runtime_error("fcntl(F_GETFL) failed");
    }
    opts |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
        throw std::runtime_error("fcntl(F_SETFL) failed");
    }
}

void Server::createSocket() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        throw std::runtime_error("Socket creation failed");
    }
    setNonBlocking(server_socket);
}

void Server::bindSocket(int port) {
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(server_socket);
        throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
    }
}

void Server::startListening() {
    const int BACKLOG = 10;
    if (listen(server_socket, BACKLOG) == -1) {
        close(server_socket);
        throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
    }
}

void Server::createEpollInstance() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        close(server_socket);
        throw std::runtime_error("epoll_create1 failed");
    }
    addSocketToEpoll(server_socket);
}

void Server::addSocketToEpoll(int sock) {
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event) == -1) {
        close(server_socket);
        close(epoll_fd);
        throw std::runtime_error("epoll_ctl: server_socket failed");
    }
}

void Server::eventLoop() {
    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            close(server_socket);
            close(epoll_fd);
            throw std::runtime_error("epoll_wait failed");
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == server_socket) {
                // Handle new connection
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
                if (client_socket == -1) {
                    std::cerr << "Accept failed\n";
                    continue;
                }
                setNonBlocking(client_socket);

                struct epoll_event event;
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == -1) {
                    close(client_socket);
                    std::cerr << "epoll_ctl: client_socket failed\n";
                    continue;
                }
                clients.insert(std::make_pair(client_socket, Client(client_socket)));
                std::cout << "New client connected.\n";
            } else {
                // Handle data from existing client
                char buffer[1024];
                int client_socket = events[n].data.fd;
                int bytes_read = read(client_socket, buffer, sizeof(buffer));
                if (bytes_read == -1) {
                    std::cerr << "Read failed\n";
                    close(client_socket);
                    clients.erase(client_socket);
                    continue;
                } else if (bytes_read == 0) {
                    std::cout << "Client disconnected.\n";
                    close(client_socket);
                    clients.erase(client_socket);
                } else {
                    buffer[bytes_read] = '\0';
                    handleClientMessage(client_socket, std::string(buffer));
                }
            }
        }
    }
}

bool Server::checkAuthentication(int client_socket) {
    std::map<int, Client>::const_iterator it = clients.find(client_socket);
    if (it == clients.end()) {
        std::cerr << "Client not found\n";
        return false;
    }

    const Client& client = it->second;

    // Check if the client has set NICK, USER, and PASS
    if (client.getNick().empty() || client.getUser().empty()) {
        std::string response = "You need to be authenticated to use this command\r\n";
        send(client_socket, response.c_str(), response.length(), 0);
        return false;
    }

    return true;
}


void Server::handleClientMessage(int client_socket, const std::string& message) {
    std::string command;
    std::string args;

    // Log the entire message for debugging
    std::cout << "Received message: " << message << std::endl;

    size_t space_pos = message.find(' ');
    if (space_pos != std::string::npos) {
        command = message.substr(0, space_pos);
        args = message.substr(space_pos + 1);
    } else {
        command = message;
    }

    std::cout << "Command: " << command << std::endl;
    std::cout << "Arguments: " << args << std::endl;


    if (command == "NICK") {
        handleNICK(client_socket, args);
    } else if (command == "USER") {
        handleUSER(client_socket, args);
    } else if (command == "PASS") {
        handlePASS(client_socket, args);
    } else if (command == "JOIN") {
        if (checkAuthentication(client_socket)) handleJOIN(client_socket, args);
    } else if (command == "PART") {
        if (checkAuthentication(client_socket)) handlePART(client_socket, args);
    } else if (command == "PRIVMSG") {
        if (checkAuthentication(client_socket)) handlePRIVMSG(client_socket, args);
    } else if (command == "KICK") {
        if (checkAuthentication(client_socket)) handleKICK(client_socket, args);
    } else if (command == "INVITE") {
        if (checkAuthentication(client_socket)) handleINVITE(client_socket, args);
    } else if (command == "TOPIC") {
        if (checkAuthentication(client_socket)) handleTOPIC(client_socket, args);
    } else if (command == "MODE") {
        if (checkAuthentication(client_socket)) handleMODE(client_socket, args);
    } else {
        handleUnknownCommand(client_socket);
    }
}


void Server::handleUnknownCommand(int client_socket) {
    std::string response = "Unknown command\r\n";
    send(client_socket, response.c_str(), response.length(), 0);
}


void Server::handleNICK(int client_socket, const std::string& args) {
    clients[client_socket].setNick(args);
    std::string response = "Nickname set\r\n";
    send(client_socket, response.c_str(), response.length(), 0);
}

void Server::handleUSER(int client_socket, const std::string& args) {
    clients[client_socket].setUser(args);
    std::string response = "User set\r\n";
    send(client_socket, response.c_str(), response.length(), 0);
}

void Server::handlePASS(int client_socket, const std::string& args) {
    if (args == password) {
        clients[client_socket].setAuthenticated(true);
        std::string response = "Password accepted\r\n";
        send(client_socket, response.c_str(), response.length(), 0);
    } else {
        std::string response = "Password incorrect\r\n";
        send(client_socket, response.c_str(), response.length(), 0);
        close(client_socket);
        clients.erase(client_socket);
    }
}

void Server::handleJOIN(int client_socket, const std::string& args) {
    std::string channel_name = args;
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);
    if (it == channels.end()) {
        Channel new_channel(channel_name);
        channels.insert(std::make_pair(channel_name, new_channel));
        it = channels.find(channel_name);
    }
    it->second.addUser(client_socket);
    std::string response = "Joined channel " + channel_name + "\r\n";
    send(client_socket, response.c_str(), response.length(), 0);
    it->second.broadcastMessage(clients[client_socket].getNick() + " has joined the channel\r\n", client_socket);
}

void Server::handlePART(int client_socket, const std::string& args) {
    std::string channel_name = args;
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);
    if (it != channels.end()) {
        it->second.removeUser(client_socket);
        std::string response = "Left channel " + channel_name + "\r\n";
        send(client_socket, response.c_str(), response.length(), 0);
        it->second.broadcastMessage(clients[client_socket].getNick() + " has left the channel\r\n", client_socket);
    } else {
        std::string response = "Unknown channel\r\n";
        send(client_socket, response.c_str(), response.length(), 0);
    }
}

void Server::handlePRIVMSG(int client_socket, const std::string& args) {
    size_t space_pos = args.find(' ');
    if (space_pos != std::string::npos) {
        std::string recipient = args.substr(0, space_pos);
        std::string msg = args.substr(space_pos + 1);

        // Check if recipient is a channel or a client
        if (recipient[0] == '#') { // Channel
            std::map<std::string, Channel>::iterator it = channels.find(recipient);
            if (it != channels.end()) {
                it->second.broadcastMessage(clients[client_socket].getNick() + ": " + msg + "\r\n", client_socket);
            } else {
                std::string response = "Unknown channel\r\n";
                send(client_socket, response.c_str(), response.length(), 0);
            }
        } else { // Client
            std::map<int, Client>::iterator it;
            for (it = clients.begin(); it != clients.end(); ++it) {
                if (it->second.getNick() == recipient) {
                    std::string response = clients[client_socket].getNick() + ": " + msg + "\r\n";
                    send(it->first, response.c_str(), response.length(), 0);
                    break;
                }
            }
        }
    }
}

void Server::handleKICK(int client_socket, const std::string& args) {
    size_t space_pos = args.find(' ');
    if (space_pos != std::string::npos) {
        std::string channel_name = args.substr(0, space_pos);
        std::string user_to_kick = args.substr(space_pos + 1);

        std::map<std::string, Channel>::iterator it = channels.find(channel_name);
        if (it != channels.end()) {
            // Check if the client is an operator
            if (it->second.isOperator(client_socket)) {
                std::map<int, Client>::iterator client_it;
                for (client_it = clients.begin(); client_it != clients.end(); ++client_it) {
                    if (client_it->second.getNick() == user_to_kick) {
                        it->second.removeUser(client_it->first);
                        std::string response = "Kicked " + user_to_kick + " from channel " + channel_name + "\r\n";
                        send(client_socket, response.c_str(), response.length(), 0);
                        it->second.broadcastMessage(user_to_kick + " has been kicked from the channel\r\n", client_it->first);
                        break;
                    }
                }
            } else {
                std::string response = "You are not an operator\r\n";
                send(client_socket, response.c_str(), response.length(), 0);
            }
        } else {
            std::string response = "Unknown channel\r\n";
            send(client_socket, response.c_str(), response.length(), 0);
        }
    }
}

void Server::handleINVITE(int client_socket, const std::string& args) {
    size_t space_pos = args.find(' ');
    if (space_pos != std::string::npos) {
        std::string channel_name = args.substr(0, space_pos);
        std::string user_to_invite = args.substr(space_pos + 1);

        std::map<std::string, Channel>::iterator it = channels.find(channel_name);
        if (it != channels.end()) {
            // Check if the client is an operator
            if (it->second.isOperator(client_socket)) {
                std::map<int, Client>::iterator client_it;
                for (client_it = clients.begin(); client_it != clients.end(); ++client_it) {
                    if (client_it->second.getNick() == user_to_invite) {
                        it->second.addUser(client_it->first);
                        std::string response = "Invited " + user_to_invite + " to channel " + channel_name + "\r\n";
                        send(client_socket, response.c_str(), response.length(), 0);
                        std::string invite_msg = "You have been invited to join " + channel_name + "\r\n";
                        send(client_it->first, invite_msg.c_str(), invite_msg.length(), 0);
                        break;
                    }
                }
            } else {
                std::string response = "You are not an operator\r\n";
                send(client_socket, response.c_str(), response.length(), 0);
            }
        } else {
            std::string response = "Unknown channel\r\n";
            send(client_socket, response.c_str(), response.length(), 0);
        }
    }
}

void Server::handleTOPIC(int client_socket, const std::string& args) {
    size_t space_pos = args.find(' ');
    if (space_pos != std::string::npos) {
        std::string channel_name = args.substr(0, space_pos);
        std::string topic = args.substr(space_pos + 1);

        std::map<std::string, Channel>::iterator it = channels.find(channel_name);
        if (it != channels.end()) {
            // Check if the client is an operator
            if (it->second.isOperator(client_socket)) {
                it->second.setTopic(topic);
                std::string response = "Topic set for channel " + channel_name + "\r\n";
                send(client_socket, response.c_str(), response.length(), 0);
                it->second.broadcastMessage("Topic changed to: " + topic + "\r\n");
            } else {
                std::string response = "You are not an operator\r\n";
                send(client_socket, response.c_str(), response.length(), 0);
            }
        } else {
            std::string response = "Unknown channel\r\n";
            send(client_socket, response.c_str(), response.length(), 0);
        }
    }
}

void Server::handleMODE(int client_socket, const std::string& args) {
    size_t space_pos = args.find(' ');
    if (space_pos != std::string::npos) {
        std::string channel_name = args.substr(0, space_pos);
        std::string mode_args = args.substr(space_pos + 1);

        std::map<std::string, Channel>::iterator it = channels.find(channel_name);
        if (it != channels.end()) {
            // Check if the client is an operator
            if (it->second.isOperator(client_socket)) {
                // Parse mode arguments
                if (mode_args.find('i') != std::string::npos) {
                    it->second.setMode("i");
                }
                if (mode_args.find('t') != std::string::npos) {
                    it->second.setMode("t");
                }
                if (mode_args.find('k') != std::string::npos) {
                    std::string password = mode_args.substr(mode_args.find('k') + 1);
                    it->second.setPassword(password);
                }
                if (mode_args.find('l') != std::string::npos) {
                    std::stringstream ss(mode_args.substr(mode_args.find('l') + 1));
                    int limit;
                    ss >> limit;
                    it->second.setLimit(limit);
                }
                std::string response = "Mode set for channel " + channel_name + "\r\n";
                send(client_socket, response.c_str(), response.length(), 0);
            } else {
                std::string response = "You are not an operator\r\n";
                send(client_socket, response.c_str(), response.length(), 0);
            }
        } else {
            std::string response = "Unknown channel\r\n";
            send(client_socket, response.c_str(), response.length(), 0);
        }
    }
}
