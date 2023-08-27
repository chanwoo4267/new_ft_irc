#include "../includes/Command.hpp"

void PassCommand::execute()
{
    printCommandMessage(2, _client_socket, "PASS " + _arg);
}

void NickCommand::execute()
{
   printCommandMessage(2, _client_socket, "NICK " + _arg);
}

void UserCommand::execute()
{
    printCommandMessage(2, _client_socket, "USER " + _arg);
}

void PrivmsgCommand::execute()
{
    printCommandMessage(2, _client_socket, "PRIVMSG " + _arg);
    // print message to client
    // std::string message = "";
    // for (std::vector<std::string>::iterator it = _args.begin() + 1; it != _args.end(); ++it)
    //     message += *it + " ";
    // _server.sendMessageToClientByNick(_args[0], message);
}