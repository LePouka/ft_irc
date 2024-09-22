#include "../includes/Server.hpp"
#include "../includes/Util.hpp"

Server::Server(int port, const std::string& password)
	: serverPassword(password), server_config_password(password) {
	serverPasswordRequired = !serverPassword.empty();
	createSocket();
	bindSocket(port);
	startListening();
	createEpollInstance();
}


Server::~Server() {
	close(server_socket);
	close(epoll_fd);
}

ChannelArray &Server::getChannelArray()
{
    return (this->channels);
}


std::map<int, Client>& Server::getClientMap() 
{
    return this->clients;
}

void Server::run() {
	eventLoop();
}

void Server::setNonBlocking(int sock) {
	int opts = fcntl(sock, F_GETFL, O_NONBLOCK);
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
					std::cerr << "accept failed: " << strerror(errno) << "\n";
					continue;
				}

				setNonBlocking(client_socket);

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = client_socket;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &ev) == -1) {
					close(client_socket);
					std::cerr << "epoll_ctl: client_socket failed: " << strerror(errno) << "\n";
					continue;
				}

				clients[client_socket] = Client(client_socket);
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
					if (errno == EAGAIN || errno == EWOULDBLOCK) {
						continue;
					} else {
						std::cerr << "read failed: " << strerror(errno) << "\n";
					}
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
	Client& client = clients[client_socket];
	size_t pos = message.find(' ');
	std::string command = (pos != std::string::npos) ? message.substr(0, pos) : message;
	std::string arg = (pos != std::string::npos) ? message.substr(pos + 1) : "";

	if (command == "PASS") {
		std::cout << "ohoh\n";
		handlePassCommand(client_socket, arg);
	} else if (command == "CAP") {
	} else if (command == "WHOIS") {
	} else if (command == "MODE") {
	} else if (command == "NICK") {
		handleNickCommand(client_socket, arg);
	} else if (command == "USER") {
		handleUserCommand(client_socket, arg);
	} else if (command == "PING") {
		std::ostringstream response;
		response << PONG_MSG("server", clients[client_socket].getNick());
		send(client_socket, response.str().c_str(), response.str().length(), 0);
	} else if (command == "JOIN"){
		arg.erase(arg.find_last_not_of(" \n\r") + 1);
		join(client, arg, *this);
	}else if (command == "PRIVMSG"){
		arg.erase(arg.find_last_not_of(" \n\r") + 1);
		privmsg(client, arg, *this);
	} else if(command == "INVITE"){
		arg.erase(arg.find_last_not_of(" \n\r") + 1);
		// invite(client, arg, *this);
	} else {
		sendErrorMessage(client_socket, command);
	}
}

void Server::sendErrorMessage(int client_socket, const std::string& command) {
	std::ostringstream error_message;
	error_message << ERR_UNKNOWNCOMMAND("server", command);
	send(client_socket, error_message.str().c_str(), error_message.str().length(), 0);
}
