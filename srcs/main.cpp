#include "../includes/irc.h"

void send_welcome_message(int client_socket, std::map<int, std::string> *client_nicks, std::map<int, std::string> *client_users) {
    if (client_nicks->find(client_socket) != client_nicks->end() && client_users->find(client_socket) != client_users->end()) {
        std::string nick = (*client_nicks)[client_socket];
        std::string welcome_message = ":server 001 " + nick + " :Welcome to the IRC server\n";
        std::string mode_message = ":server 221 " + nick + " +i\n";
        send(client_socket, welcome_message.c_str(), welcome_message.length(), 0);
        send(client_socket, mode_message.c_str(), mode_message.length(), 0);
    }
}

void handle_client_message(int client_socket, const std::string& message, std::map<int, std::string> *client_nicks, std::map<int, std::string> *client_users) {
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
        (*client_nicks)[client_socket] = nick;
        send_welcome_message(client_socket, client_nicks, client_users);
    } else if (command == "USER") {
        std::string user = message.substr(pos + 1);
        (*client_users)[client_socket] = user;
        send_welcome_message(client_socket, client_nicks, client_users);
    } else if (command == "MODE") {
        std::string mode_response = ":server 221 " + (*client_nicks)[client_socket] + " +i\n";
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
                std::string ctcp_response = "NOTICE " + (*client_nicks)[client_socket] + " :\x01PING " + timestamp + "\x01\n";
                send(client_socket, ctcp_response.c_str(), ctcp_response.length(), 0);
            } else {
                std::string privmsg_response = ":" + (*client_nicks)[client_socket] + " PRIVMSG " + target + " :" + msg + "\n";
                send(client_socket, privmsg_response.c_str(), privmsg_response.length(), 0);
            }
        } else {
            std::string unknown_response = ":server 421 " + (*client_nicks)[client_socket] + " " + command + " :Unknown command\n";
            send(client_socket, unknown_response.c_str(), unknown_response.length(), 0);
        }
    } else if (command == "QUIT") {
        std::string quit_message = message.substr(pos + 1);
        std::string quit_response = ":" + (*client_nicks)[client_socket] + " QUIT :" + quit_message + "\n";
        send(client_socket, quit_response.c_str(), quit_response.length(), 0);

        close(client_socket);
        client_nicks->erase(client_socket);
        client_users->erase(client_socket);
        std::cout << "Client " << client_socket << " disconnected: " << quit_message << std::endl;
    } else if (command == "WHOIS") {
        std::string whois_nick = message.substr(pos + 1);
        bool user_found = false;

        for (std::map<int, std::string>::iterator it = client_nicks->begin(); it != client_nicks->end(); ++it) {
            if (it->second == whois_nick) {
                user_found = true;
                std::string whois_response = ":server 311 " + (*client_nicks)[client_socket] + " " + it->second + " " + it->second + " localhost * :" + it->second + "\n";
                send(client_socket, whois_response.c_str(), whois_response.length(), 0);
                std::string whois_end_response = ":server 318 " + (*client_nicks)[client_socket] + " " + it->second + " :End of WHOIS list\n";
                send(client_socket, whois_end_response.c_str(), whois_end_response.length(), 0);
                break;
            }
        }

        if (!user_found) {
            std::string no_such_nick_response = ":server 401 " + (*client_nicks)[client_socket] + " " + whois_nick + " :No such nick/channel\n";
            send(client_socket, no_such_nick_response.c_str(), no_such_nick_response.length(), 0);
        }
    } else {
        std::string unknown_response = ":server 421 " + (*client_nicks)[client_socket] + " " + command + " :Unknown command\n";
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
        socket_binding(&server_addr, &server_socket);

        // Start listening for connections
        start_listening(&server_socket);

        // Create an epoll instance
        instance_creation(&epoll_fd, &server_socket);

        // Add the server socket to the epoll instance
        socket_to_instance(&server_socket, &epoll_fd, &ev);

        std::cout << "IRC server started on port " << PORT << "\n";

        // Event loop
        

        close(server_socket);
        close(epoll_fd);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}