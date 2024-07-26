#include "../includes/Server.hpp"

int main()
{
    try {
        Server server(PORT);
        std::cout << "IRC server started on port " << PORT << "\n";
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
