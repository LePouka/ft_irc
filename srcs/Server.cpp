#include "../includes/Server.hpp"
#include "../includes/Util.hpp"

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
            throw std::runtime_error("epoll_wait failed");
        }

        for (int i = 0; i < nfds; ++i) {
            int fd = events[i].data.fd;

            if (fd == server_socket) {
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

                clients[client_socket] = Client(client_socket);

                // Pas d'envoi de message de bienvenue ici
                std::cout << "New client connected.\n";
            } else {
                int client_socket = fd;
                char buffer[1024];
                ssize_t bytes_read;
                std::string message_buffer;

                while ((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytes_read] = '\0';
                    message_buffer += buffer;

                    size_t pos;
                    while ((pos = message_buffer.find("\r\n")) != std::string::npos) {
                        std::string message = message_buffer.substr(0, pos);
                        message_buffer.erase(0, pos + 2);

                        handleClientMessage(client_socket, message);
                    }
                }

                if (bytes_read == -1) {
                    std::cerr << "read failed\n";
                } else if (bytes_read == 0) {
                    std::cout << "Client disconnected.\n";
                    close(client_socket);
                    clients.erase(client_socket);
                }
            }
        }
    }
}

void Server::handleClientMessage(int client_socket, const std::string& message) {
    std::cout << "Received from client " << client_socket << ": " << message << std::endl;

    size_t pos = message.find(' ');
    std::string command = (pos != std::string::npos) ? message.substr(0, pos) : message;
    std::string arg = (pos != std::string::npos) ? message.substr(pos + 1) : "";

    if (command == "CAP") {
        // Handle CAP command
    } else if (command == "WHOIS") {
        // Handle WHOIS command
    } else if (command == "MODE") {
        // Handle MODE command
    } else if (command == "NICK") {
        if (arg.empty()) {
            std::string error_message = ERR_NONICKNAMEGIVEN("server");
            send(client_socket, error_message.c_str(), error_message.length(), 0);
        } else {
            handleNickCommand(client_socket, arg);
        }
    } else if (command == "USER") {
        handleUserCommand(client_socket, arg);
    } else if (command == "PING") {
        std::ostringstream response;
        response << PONG_MSG("server", clients[client_socket].getNick());
        send(client_socket, response.str().c_str(), response.str().length(), 0);
    } else {
        sendErrorMessage(client_socket, command);
    }
}

void Server::handleNickCommand(int client_socket, const std::string& new_nick) {
    if (new_nick.empty()) {
        // Erreur : aucun pseudo donné
        std::string error_message = ERR_NONICKNAMEGIVEN("server");
        send(client_socket, error_message.c_str(), error_message.length(), 0);
        return;
    }

    // Vérifier si le pseudo est déjà pris
    std::map<int, Client>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.getNick() == new_nick) {
            // Erreur : pseudo déjà utilisé
            std::ostringstream error_message;
            error_message << ERR_NICKNAMEINUSE("server", new_nick);
            send(client_socket, error_message.str().c_str(), error_message.str().length(), 0);
            return;
        }
    }

    // Obtenir le vieux pseudo
    std::string old_nick = clients[client_socket].getNick();

    // Mettre à jour le pseudo du client
    clients[client_socket].setNick(new_nick);

    // Préparer le message de notification pour les autres clients
    std::ostringstream notification_message;
    notification_message << ":" << old_nick << " NICK " << new_nick << "\r\n";
    std::string notification = notification_message.str();

    // Envoyer la notification aux autres clients
    for (it = clients.begin(); it != clients.end(); ++it) {
        if (it->first != client_socket) {
            send(it->first, notification.c_str(), notification.length(), 0);
        }
    }

    // Envoyer la confirmation au client dont le pseudo a changé
    std::ostringstream response;
    response << ":server 001 " << clients[client_socket].getNick() << " :You're now known as " << new_nick << "\r\n";
    std::string response_str = response.str();

    if (send(client_socket, response_str.c_str(), response_str.length(), 0) == -1) {
        std::cerr << "Error sending nickname change confirmation to client " << client_socket << std::endl;
    }
}

void Server::handleUserCommand(int client_socket, const std::string& user) {
    if (user.empty()) {
        std::string error_message = ERR_NEEDMOREPARAMS("server", "USER");
        send(client_socket, error_message.c_str(), error_message.length(), 0);
        return;
    }

    clients[client_socket].setUser(user);
    std::ostringstream response;
    response << ":server 001 " << clients[client_socket].getNick() << " :User set to " << user << "\r\n";
    send(client_socket, response.str().c_str(), response.str().length(), 0);
}

void Server::sendErrorMessage(int client_socket, const std::string& command) {
    std::ostringstream error_message;
    error_message << ERR_UNKNOWNCOMMAND("server", command);
    send(client_socket, error_message.str().c_str(), error_message.str().length(), 0);
}
