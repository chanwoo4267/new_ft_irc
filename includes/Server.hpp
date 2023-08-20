#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "../includes/ClientManager.hpp"
class ClientManager;

class Server
{
    private:

        int                         _server_port;
        int                         _server_socket;
        char*                       _server_password;
        struct sockaddr_in          _server_address;

        ClientManager&              _client_manager;

        // kqueue variables
        int                         _kq;
        std::vector<struct kevent>  _change_list;
        struct kevent               _event_list[16];

        void                        initServer();
        void                        initKqueue();
        void                        addEvent(std::vector<struct kevent>& change_list, uintptr_t ident, \
                                            int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
        void                        systemError(const char* msg, int exit_code);

        void                        parseAndExecuteCommand(int client_socket);

    public:

        Server(int port, char* password, ClientManager& client_manager);
        ~Server();

        void                        runServer();
};

#endif