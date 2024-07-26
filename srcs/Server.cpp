#include "../includes/Server.hpp"

Server::Server(int port)
{
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
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0) {
        throw std::runtime_error("fcntl(F_SETFL) failed");
    }
}

void Server::createSocket() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        throw std::runtime_error("socket creation failed");
    }
    setNonBlocking(server_socket);
}

void Server::bindSocket(int port) {
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(server_socket);
        std::string error_msg = "bind failed: " + std::string(strerror(errno));
        throw std::runtime_error(error_msg);
    }
}

void Server::startListening() {
    const int BACKLOG = 10;

    if (listen(server_socket, BACKLOG) == -1) {
        close(server_socket);
        std::string error_msg = "listen failed: " + std::string(strerror(errno));
        throw std::runtime_error(error_msg);
    }
}

void Server::createEpollInstance()
{
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        close(server_socket);
        throw std::runtime_error("epoll_create1 failed");
    }
    addSocketToEpoll(server_socket);
}

void Server::addSocketToEpoll(int sock) {
    ev.events = EPOLLIN;
    ev.data.fd = sock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev) == -1) {
        close(server_socket);
        close(epoll_fd);
        throw std::runtime_error("epoll_ctl: server_socket failed");
    }
}

void Server::eventLoop() {
    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            close(server_socket);
            close(epoll_fd);
            throw std::runtime_error("epoll_wait failed");
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == server_socket) {
                sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
                if (client_socket == -1) {
                    std::cerr << "accept failed\n";
                    continue;
                }
                setNonBlocking(client_socket);

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &ev) == -1) {
                    close(client_socket);
                    std::cerr << "epoll_ctl: client_socket failed\n";
                    continue;
                }
                clients.insert(std::make_pair(client_socket, Client(client_socket))); // Utilisation de insert
                std::cout << "New client connected.\n";
            } else {
                char buffer[1024];
                int client_socket = events[n].data.fd;
                int bytes_read = read(client_socket, buffer, sizeof(buffer));
                if (bytes_read == -1) {
                    std::cerr << "read failed\n";
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


void Server::handleClientMessage(int client_socket, const std::string& message) {
    Client& client = clients[client_socket];
    std::cout << "Received from client " << client_socket << ": " << message << std::endl;

    size_t pos = message.find(' ');
    std::string command = (pos != std::string::npos) ? message.substr(0, pos) : message;
    std::string params = (pos != std::string::npos) ? message.substr(pos + 1) : "";

    if (command == "NICK") {
        params.erase(params.find_last_not_of(" \n\r") + 1);
        client.setNick(params);
        sendWelcomeMessage(client_socket);
    } else if (command == "USER") {
        size_t user_end = params.find(' ');
        if (user_end != std::string::npos) {
            std::string user = params.substr(0, user_end);
            params.erase(0, user_end + 1);
            std::string realname = params;
            realname.erase(realname.find_last_not_of(" \n\r") + 1);
            client.setUser(user + " " + realname);
            sendWelcomeMessage(client_socket);
        } else {
            sendErrorMessage(client_socket, command);
        }
    } else if (command == "PING") {
        std::string pong_response = "PONG " + params + "\r\n";
        send(client_socket, pong_response.c_str(), pong_response.length(), 0);
    } else {
        sendErrorMessage(client_socket, command);
    }
}

void Server::sendErrorMessage(int client_socket, const std::string& command) {
    // Code d'erreur 421 : Unknown command
    std::string error_message = ":server 421 " + clients[client_socket].getNick() + " " + command + " :Unknown command\r\n";
    send(client_socket, error_message.c_str(), error_message.length(), 0);
}

void Server::sendWelcomeMessage(int client_socket) {
    Client& client = clients[client_socket];
    if (!client.getNick().empty() && !client.getUser().empty()) {
        std::string welcome_message = ":server 001 " + client.getNick() + " :Welcome to the IRC server\n";
        std::string mode_message = ":server 221 " + client.getNick() + " +i\n";
        send(client_socket, welcome_message.c_str(), welcome_message.length(), 0);
        send(client_socket, mode_message.c_str(), mode_message.length(), 0);
    }
}
