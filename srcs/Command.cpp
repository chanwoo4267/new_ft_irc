#include "../includes/Command.hpp"

// arg는 명령어를 제외한 나머지 부분이 한 문자열로 들어감
// 필요하다면 파싱하여 사용할 것

void PassCommand::execute()
{
    printServerMessage(2, "PASS " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket)) // client is connected to server
    {
        if (_server.getPassword() == _arg) // correct password
        {
            _client_manager.setClientAuthenticatedBySocket(_client_socket, true);
            printCommandMessage(2, _client_socket, "Password correct");
        }
        else
            printCommandMessage(1, _client_socket, "Password incorrect");
    }
    else
        printCommandMessage(1, _client_socket, "Not connected to server");
}

void NickCommand::execute()
{
    printServerMessage(2, "NICK " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket)) // client is connected to server
    {
        if (_client_manager.isClientAuthenticatedBySocket(_client_socket)) // client is authenticated
        {
            if (_arg.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") != std::string::npos) // invalid nickname
                printCommandMessage(1, _client_socket, "Invalid nickname");
            else
            {
                if (_client_manager.isClientExistByNick(_arg)) // duplicated nickname
                    printCommandMessage(1, _client_socket, "Nickname already in use");
                else
                {
                    _client_manager.setClientNicknameBySocket(_client_socket, _arg);
                    printCommandMessage(2, _client_socket, "Nickname set");
                }
            }
        }
        else
            printCommandMessage(1, _client_socket, "Not authenticated");
    }
    else
        printCommandMessage(1, _client_socket, "Not connected to server");
}

void UserCommand::execute()
{
    printCommandMessage(2, _client_socket, "USER " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket)) // client is connected to server
    {
        if (_client_manager.isClientAuthenticatedBySocket(_client_socket)) // client is authenticated
        {
            std::vector<std::string> args = splitString(_arg, ' ');
            if (args.size() != 4) // invalid number of arguments
                printCommandMessage(1, _client_socket, "Invalid number of arguments");
            else
            {
                if (!args[0].empty() && !args[1].empty() && !args[2].empty() && !args[3].empty()) // empty parameter argument
                {
                    _client_manager.setClientNamesBySocket(_client_socket, args[0], args[1], args[2], args[3]);
                    printCommandMessage(2, _client_socket, "User registered");
                }
                else
                    printCommandMessage(1, _client_socket, "Cannot have empty parameter");
            }
        }
        else
            printCommandMessage(1, _client_socket, "Not authenticated");
    }
    else
        printCommandMessage(1, _client_socket, "Not connected to server");
}

void PrivmsgCommand::execute()
{
    printCommandMessage(2, _client_socket, "PRIVMSG " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket)) // client is connected to server
    {
        if (_client_manager.isClientAuthenticatedBySocket(_client_socket)) // client is authenticated
        {
            size_t pos = _arg.find(' ');
            if (pos != std::string::npos) // argument has target and message
            {
                std::string target = _arg.substr(0, pos);
                std::string message = _arg.substr(pos + 1);
                std::string sender_nick = _client_manager.getClientNicknameBySocket(_client_socket);

                if (target[0] == '#') // channel message
                {
                    if (_channel_manager.isChannelExist(target)) // target channel exists
                    {
                        if (_channel_manager.isClientInChannel(target, sender_nick)) // sender client is in channel
                        {
                            _server.sendMessageToChannel(sender_nick, target, sender_nick + " -> " + message);
                            printCommandMessage(2, _client_socket, "Message sent");
                        }
                        else
                            printCommandMessage(1, _client_socket, "You are not in channel");
                    }
                    else
                        printCommandMessage(1, _client_socket, "Channel does not exist");
                }
                else // private message
                {
                    if (_client_manager.isClientExistByNick(target)) // target client exists
                    {
                        if (target == sender_nick) // cannot send message to yourself
                            printCommandMessage(1, _client_socket, "Cannot send message to yourself");
                        else
                        {
                            _server.sendMessageToClientByNick(target, sender_nick + " -> " + message);
                            printCommandMessage(2, _client_socket, "Message sent");
                        } 
                    }
                    else
                        printCommandMessage(1, _client_socket, "Client does not exist");
                }
            }
            else
                printCommandMessage(1, _client_socket, "Invalid number of arguments");
        }
        else
            printCommandMessage(1, _client_socket, "Not authenticated");
    }
    else
        printCommandMessage(1, _client_socket, "Not connected to server");
}