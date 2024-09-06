#include "../includes/Server.hpp"

int main(int argc, char* argv[]) {
	try {
		if (argc < 3) {
			std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
			return EXIT_FAILURE;
		}
		int port = std::atoi(argv[1]);
		if (port <= 0 || port > 65535) {
			std::cerr << "Invalid port number. Must be between 1 and 65535.\n";
			return EXIT_FAILURE;
		}
		std::string password = argv[2];
		Server server(port, password);
		std::cout << "IRC server started on port " << port << "\n";

		server.run();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
