#pragma once

#include <map>
#include <string>
#include <sys/epoll.h>
#include "Client.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <stdexcept>

#define SERVER_NAME "ircserv"

