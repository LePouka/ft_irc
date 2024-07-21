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
#define PORT 6667


//MAIN.CPP
void handle_client_message(int client_socket, const std::string& message,     std::map<int, std::string> *client_nicks, std::map<int, std::string> *client_users);

//SOCKET.CPP
void set_nonblocking(int sock);
void socket_creation(int *server_socket);
void socket_binding(sockaddr_in *server_addr, int *server_socket);
void start_listening(int *server_socket);

//EPOLL.CPP
void instance_creation(int *epoll_fd, int *server_socket);
void socket_to_instance(int *server_socket, int *epoll_fd, epoll_event *ev);
void event_loop(int *server_socket, int *epoll_fd, epoll_event *events, epoll_event *ev);
