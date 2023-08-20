#include "../includes/Server.hpp"
#include "../includes/ClientManager.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
        std::cerr << "Usage: ./ircserv [port] [password]" << std::endl;
    else
    {
        ClientManager client_manager;
        Server server(atoi(argv[1]), argv[2], client_manager);
        server.runServer();
    }
    return 0;
}