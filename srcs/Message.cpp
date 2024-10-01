#include "../includes/Message.hpp"

void sendMessage(int clientSocket, std::string msg)
{
	const char* msgC = msg.c_str();
	size_t msgLength = msg.size();
	ssize_t bytesSent = 0;
	size_t totalSent = 0;
	while (totalSent < msgLength) {
		bytesSent = send(clientSocket, msgC + totalSent, msgLength - totalSent, MSG_NOSIGNAL);
		if (bytesSent == -1) {
			perror("send failed");
			break;
		}
		totalSent += bytesSent;
	}
}
