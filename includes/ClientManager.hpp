#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../includes/Client.hpp"
class Client;

class ClientManager
{
    private:
        std::vector<Client>     _client_list;

        Client&                 getClientBySocket(int client_socket);
        Client&                 getClientByNick(std::string nickname);
    
    public:

        /* client management methods */

        void                    addClient(int client_socket);
        void                    deleteClientBySocket(int client_socket);

        /* client set methods */

        void                    setClientNicknameBySocket(int client_socket, std::string nickname);
        void                    setClientAuthenticatedBySocket(int client_socket, bool authenticated);
        void                    setClientNamesBySocket(int client_socket, std::string username, std::string hostname, std::string realname, std::string servername);
        void                    setClientFirstConnectBySocket(int client_socket, bool first_connect);

        /* get methods */

        int                     getClientSocketByNick(std::string nickname);
        std::string             getClientNicknameBySocket(int client_socket);
        std::string             getClientUsernameBySocket(int client_socket);
        std::string             getClientHostnameBySocket(int client_socket);

        /* client buffer methods */

        void                    appendReadBufferBySocket(int client_socket, std::string str);
        void                    appendWriteBufferBySocket(int client_socket, std::string str);
        void                    setReadBufferBySocket(int client_socket, std::string str);
        void                    setWriteBufferBySocket(int client_socket, std::string str);
        std::string             getReadBufferBySocket(int client_socket);
        std::string             getWriteBufferBySocket(int client_socket);

        /* check methods */

        bool                    isReadBufferEndWithCRLF(int client_socket);
        bool                    isClientExistBySocket(int client_socket);
        bool                    isClientExistByNick(std::string nickname);
        bool                    isClientAuthenticatedBySocket(int client_socket);
        bool                    isClientReadyBySocket(int client_socket);
        bool                    isClientFirstConnectBySocket(int client_socket);
        bool                    isClientInvitedBySocket(int client_socket, std::string channel);

        /* invite methods */

        void                    inviteClientToChannelByNick(std::string nickname, std::string channel);
};

#endif