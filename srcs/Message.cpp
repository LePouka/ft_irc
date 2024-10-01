#include "../includes/Message.hpp"

// void sendMessage(int clientSocket, std::string msg)
// {
//     std::cout <<"capasse\n";
//     const char* msgC = msg.c_str();
//     size_t msgLength = strlen(msgC) + 1;
//     for (size_t i = 0; i <= msgLength; ++i)
//     {
//         std::cout << msgC[i];
//     }
//     std::cout << "\n";
//     ssize_t bytesSent = send(clientSocket, msgC, msgLength, 0);

//     if (bytesSent == -1) {
//         perror("send failed");
//     }
// }

void sendMessage(int clientSocket, std::string msg)
{
	const char* msgC = msg.c_str();
	size_t msgLength = msg.size();  // Correct length without null terminator

	// Debugging: Print the message character by character
	std::cout << "Message sent : \n";
	for (size_t i = 0; i < msgLength; ++i)
	{
		std::cout << msgC[i];
	}
	std::cout << "\n";

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
