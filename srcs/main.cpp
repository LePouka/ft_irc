#include "../includes/irc.h"

std::map<int, std::string> client_nicks;
std::map<int, std::string> client_users;

void send_welcome_message(int client_socket) {
    if (client_nicks.find(client_socket) != client_nicks.end() && client_users.find(client_socket) != client_users.end()) {
        std::string nick = client_nicks[client_socket];
        std::string welcome_message = ":server 001 " + nick + " :Welcome to the IRC server\n";
        std::string mode_message = ":server 221 " + nick + " +i\n";
        send(client_socket, welcome_message.c_str(), welcome_message.length(), 0);
        send(client_socket, mode_message.c_str(), mode_message.length(), 0);
    }
}

void handle_client_message(int client_socket, const std::string& message) {
    std::cout << "Received from client " << client_socket << ": " << message << std::endl;

    std::string command;
    size_t pos = message.find(' ');
    if (pos != std::string::npos) {
        command = message.substr(0, pos);
    } else {
        command = message;
    }

    if (command == "CAP") {
        if (message.substr(pos + 1) == "LS") {
            std::string cap_response = "CAP * LS :multi-prefix\n";
            send(client_socket, cap_response.c_str(), cap_response.length(), 0);
        } else if (message.substr(pos + 1) == "END") {
            std::string cap_end_response = "CAP END\n";
            send(client_socket, cap_end_response.c_str(), cap_end_response.length(), 0);
        }
    } else if (command == "NICK") {
        std::string nick = message.substr(pos + 1);
        client_nicks[client_socket] = nick;
        send_welcome_message(client_socket);
    } else if (command == "USER") {
        std::string user = message.substr(pos + 1);
        client_users[client_socket] = user;
        send_welcome_message(client_socket);
    } else if (command == "MODE") {
        std::string mode_response = ":server 221 " + client_nicks[client_socket] + " +i\n";
        send(client_socket, mode_response.c_str(), mode_response.length(), 0);
    } else if (command == "PING") {
        std::string pong_response = "PONG " + message.substr(pos + 1) + "\n";
        send(client_socket, pong_response.c_str(), pong_response.length(), 0);
    } else if (command == "PRIVMSG") {
        size_t target_pos = message.find(' ', pos + 1);
        size_t msg_pos = message.find(" :", target_pos);
        if (target_pos != std::string::npos && msg_pos != std::string::npos) {
            std::string target = message.substr(pos + 1, target_pos - pos - 1);
            std::string msg = message.substr(msg_pos + 2);

            if (msg[0] == '\x01' && msg.substr(1, 4) == "PING") {
                std::string timestamp = msg.substr(6, msg.length() - 7);
                std::string ctcp_response = "NOTICE " + client_nicks[client_socket] + " :\x01PING " + timestamp + "\x01\n";
                send(client_socket, ctcp_response.c_str(), ctcp_response.length(), 0);
            } else {
                std::string privmsg_response = ":" + client_nicks[client_socket] + " PRIVMSG " + target + " :" + msg + "\n";
                send(client_socket, privmsg_response.c_str(), privmsg_response.length(), 0);
            }
        } else {
            std::string unknown_response = ":server 421 " + client_nicks[client_socket] + " " + command + " :Unknown command\n";
            send(client_socket, unknown_response.c_str(), unknown_response.length(), 0);
        }
    } else if (command == "QUIT") {
        std::string quit_message = message.substr(pos + 1);
        std::string quit_response = ":" + client_nicks[client_socket] + " QUIT :" + quit_message + "\n";
        send(client_socket, quit_response.c_str(), quit_response.length(), 0);

        close(client_socket);
        client_nicks.erase(client_socket);
        client_users.erase(client_socket);
        std::cout << "Client " << client_socket << " disconnected: " << quit_message << std::endl;
    } else if (command == "WHOIS") {
        std::string whois_nick = message.substr(pos + 1);
        bool user_found = false;

        for (std::map<int, std::string>::iterator it = client_nicks.begin(); it != client_nicks.end(); ++it) {
            if (it->second == whois_nick) {
                user_found = true;
                std::string whois_response = ":server 311 " + client_nicks[client_socket] + " " + it->second + " " + it->second + " localhost * :" + it->second + "\n";
                send(client_socket, whois_response.c_str(), whois_response.length(), 0);
                std::string whois_end_response = ":server 318 " + client_nicks[client_socket] + " " + it->second + " :End of WHOIS list\n";
                send(client_socket, whois_end_response.c_str(), whois_end_response.length(), 0);
                break;
            }
        }

        if (!user_found) {
            std::string no_such_nick_response = ":server 401 " + client_nicks[client_socket] + " " + whois_nick + " :No such nick/channel\n";
            send(client_socket, no_such_nick_response.c_str(), no_such_nick_response.length(), 0);
        }
    } else {
        std::string unknown_response = ":server 421 " + client_nicks[client_socket] + " " + command + " :Unknown command\n";
        send(client_socket, unknown_response.c_str(), unknown_response.length(), 0);
    }
}

int main() {
    try {
        int server_socket = 0;
        int epoll_fd = 0;
        struct sockaddr_in server_addr;
        struct epoll_event ev, events[MAX_EVENTS];

        socket_creation(&server_socket);

        // Bind the socket to an address and port
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            close(server_socket);
            std::string error_msg = "bind failed: " + std::string(strerror(errno));
            throw std::runtime_error(error_msg);
        }

        // Start listening for connections
        if (listen(server_socket, 10) == -1) {
            close(server_socket);
            throw std::runtime_error("listen failed");
        }

        // Create an epoll instance
        epoll_fd = epoll_create1(0);
        if (epoll_fd == -1) {
            close(server_socket);
            throw std::runtime_error("epoll_create1 failed");
        }

        // Add the server socket to the epoll instance
        ev.events = EPOLLIN;
        ev.data.fd = server_socket;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &ev) == -1) {
            close(server_socket);
            close(epoll_fd);
            throw std::runtime_error("epoll_ctl: server_socket failed");
        }

        std::cout << "IRC server started on port " << PORT << "\n";

        // Event loop
        while (true) {
            int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
            if (nfds == -1) {
                close(server_socket);
                close(epoll_fd);
                throw std::runtime_error("epoll_wait failed");
            }

            for (int n = 0; n < nfds; ++n) {
                if (events[n].data.fd == server_socket) {
                    // Handle new connections
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
                    if (client_socket == -1) {
                        std::cerr << "accept failed\n";
                        continue;
                    }
                    set_nonblocking(client_socket);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = client_socket;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &ev) == -1) {
                        close(client_socket);
                        std::cerr << "epoll_ctl: client_socket failed\n";
                        continue;
                    }
                    std::cout << "New client connected.\n";
                } else {
                    // Handle data from a connected client
                    char buffer[1024];
                    int client_socket = events[n].data.fd;
                    int bytes_read = read(client_socket, buffer, sizeof(buffer));
                    if (bytes_read == -1) {
                        std::cerr << "read failed\n";
                        close(client_socket);
                        continue;
                    } else if (bytes_read == 0) {
                        // Client disconnected
                        std::cout << "Client disconnected.\n";
                        close(client_socket);
                        client_nicks.erase(client_socket);
                        client_users.erase(client_socket);
                    } else {
                        // Null-terminate and process the message
                        buffer[bytes_read] = '\0';
                        std::string message(buffer);
                        handle_client_message(client_socket, message);
                    }
                }
            }
        }

        close(server_socket);
        close(epoll_fd);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}