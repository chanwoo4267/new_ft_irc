#include "../includes/Command.hpp"

// arg는 명령어를 제외한 나머지 부분이 한 문자열로 들어감
// 필요하다면 파싱하여 사용할 것

/**
 * @brief authenticate client with password
 * 
 * @note
 * client가 서버에 연결되어 있고, 올바른 password를 입력한 경우에만 인증됨
*/
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
            if (_arg.find(' ') != std::string::npos) // nickname doesn't contains space
                printCommandMessage(1, _client_socket, "Nickname cannot contain space");
            else
            {
                if (_client_manager.isClientExistByNick(_arg)) // duplicated nickname
                    printCommandMessage(1, _client_socket, "Nickname already in use");
                else
                {
                    _client_manager.setClientNicknameBySocket(_client_socket, _arg); // set nickname
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