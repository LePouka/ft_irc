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


//SOCKET.CPP
void set_nonblocking(int sock);
void socket_creation(int *server_socket);