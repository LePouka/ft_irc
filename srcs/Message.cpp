#include "../includes/Message.hpp"

void sendMessage( int clientSocket, std::string msg)
{
    const char* msgC = msg.c_str();

    size_t msgLength = strlen(msgC) + 1;

    ssize_t bytesSent = send(clientSocket, msgC, msgLength, 0);

    if (bytesSent == -1) {
        perror("send failed");
    }
}