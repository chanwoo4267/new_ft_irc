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
    
    public:
        
        void                    addClient(int client_socket);
        void                    deleteClientBySocket(int client_socket);
        void                    deleteClientByNick(std::string nickname);
        bool                    isClientExistBySocket(int client_socket);
        bool                    isClientExistByNick(std::string nickname);
        Client&                 getClientBySocket(int client_socket);
        Client&                 getClientByNick(std::string nickname);

        void                    appendReadBufferBySocket(int client_socket, std::string str);
        void                    appendWriteBufferBySocket(int client_socket, std::string str);
        void                    clearReadBufferBySocket(int client_socket);
        void                    clearWriteBufferBySocket(int client_socket);
        void                    setReadBufferBySocket(int client_socket, std::string str);
        void                    setWriteBufferBySocket(int client_socket, std::string str);
        std::string             getReadBufferBySocket(int client_socket);
        std::string             getWriteBufferBySocket(int client_socket);
        bool                    isReadBufferEndWithCRLF(int client_socket);
};

#endif