#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "../includes/Server.hpp"

// Définir le port du serveur et le mot de passe ici
const int PORT = 6667;
const std::string PASSWORD = "yourpassword"; // Remplacez par le mot de passe souhaité

int main() {
    try {
        // Création et démarrage du serveur
        Server server(PORT, PASSWORD);
        std::cout << "IRC server started on port " << PORT << "\n";
        
        // Exécution du serveur
        server.run();
    } catch (const std::exception& e) {
        // Gestion des exceptions et affichage des messages d'erreur
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
