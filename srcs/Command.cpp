#include "../includes/Command.hpp"

// arg는 명령어를 제외한 나머지 부분이 한 문자열로 들어감
// 필요하다면 파싱하여 사용할 것

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