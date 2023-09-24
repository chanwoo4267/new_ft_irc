#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/poll.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../includes/Utility.hpp"
#include "../includes/ClientManager.hpp"
#include "../includes/ChannelManager.hpp"
#include "../includes/Command.hpp"
class Command;

#define BLACK "\033[0;30m"
#define RED   "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE   "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN   "\033[0;36m"
#define WHITE  "\033[0;37m"
#define DEFAULT "\033[0m"

#define MSG_NOSIGNAL 0x4000

class Server
{
    private:

        int                         _server_port;
        int                         _server_socket;
        std::string                 _server_password;
        struct sockaddr_in          _server_address;

        ClientManager               _client_manager;
        ChannelManager              _channel_manager;

        // kqueue variables
    
        int                         _kq;
        std::vector<struct kevent>  _change_list;
        struct kevent               _event_list[16];

        // private functions

        void                        initServer();
        void                        initKqueue();
        void                        addEvent(std::vector<struct kevent>& change_list, uintptr_t ident, \
                                            int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
        void                        addPrintEvent(uintptr_t ident, std::string message);
        void                        systemError(const char* msg, int exit_code);
        Command*                    createCommand(int client_socket, std::string command, std::string arg);
        void                        parseAndExecuteCommand(int client_socket);

    public:

        Server(int port, std::string password);
        ~Server();
        void                        runServer();

        void                        sendMessageToClientByNick(std::string nick, std::string message);
        void                        sendMessageToClientBySocket(int socket, std::string message);
        void                        sendMessageToChannel(std::string nick, std::string channel, std::string message);
        void                        sendMessageToChannelExceptMe(std::string nick, std::string channel, std::string message);

        std::string                 getPassword() const;
};

#endif