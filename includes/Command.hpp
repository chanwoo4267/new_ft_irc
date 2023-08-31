#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../includes/Server.hpp"
#include "../includes/Utility.hpp"

class Server;
class ClientManager;
class ChannelManager;

class Command
{
    protected:
        int                         _client_socket;
        Server&                     _server;
        ClientManager&              _client_manager;
        ChannelManager&             _channel_manager;
        std::string                 _arg;
    
    public:
        Command(int client_socket, Server& server, ClientManager& cm, ChannelManager& chm, std::string arg) : _client_socket(client_socket), _server(server), _client_manager(cm), _channel_manager(chm), _arg(arg) {}
        virtual ~Command() {}
        virtual void execute() = 0;
};

class PassCommand : public Command
{
    public:
        PassCommand(int client_socket, Server& server, ClientManager& cm, ChannelManager& chm, std::string arg) : Command(client_socket, server, cm, chm, arg) {}
        void execute();
};

class NickCommand : public Command
{
    public:
        NickCommand(int client_socket, Server& server, ClientManager& cm, ChannelManager& chm, std::string arg) : Command(client_socket, server, cm, chm, arg) {}
        void execute();
};

class UserCommand : public Command
{
    public:
        UserCommand(int client_socket, Server& server, ClientManager& cm, ChannelManager& chm, std::string arg) : Command(client_socket, server, cm, chm, arg) {}
        void execute();
};

class PrivmsgCommand : public Command
{
    public:
        PrivmsgCommand(int client_socket, Server& server, ClientManager& cm, ChannelManager& chm, std::string arg) : Command(client_socket, server, cm, chm, arg) {}
        void execute();
};

class JoinCommand : public Command
{
    public:
        JoinCommand(int client_socket, Server& server, ClientManager& cm, ChannelManager& chm, std::string arg) : Command(client_socket, server, cm, chm, arg) {}
        void execute();
};

#endif