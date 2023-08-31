#include "../includes/Command.hpp"

// arg는 명령어를 제외한 나머지 부분이 한 문자열로 들어감
// 필요하다면 파싱하여 사용할 것

void PassCommand::execute()
{
    printServerMessage(2, "PASS " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_server.getPassword() != _arg) // incorrect password
    {
        printCommandMessage(1, _client_socket, "Password incorrect");
        return;
    }

    _client_manager.setClientAuthenticatedBySocket(_client_socket, true);
    printCommandMessage(2, _client_socket, "Password correct");
}

void NickCommand::execute()
{
    printServerMessage(2, "NICK " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientAuthenticatedBySocket(_client_socket) == false) // client is not authenticated
    {
        printCommandMessage(1, _client_socket, "Not authenticated");
        return;
    }

    if (_arg.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") != std::string::npos) // invalid nickname
    {
        printCommandMessage(1, _client_socket, "Invalid nickname");
        return;
    }

    if (_client_manager.isClientExistByNick(_arg)) // duplicated nickname
    {
        printCommandMessage(1, _client_socket, "Nickname already in use");
        return;
    }

    _client_manager.setClientNicknameBySocket(_client_socket, _arg);
    printCommandMessage(2, _client_socket, "Nickname set");
}

void UserCommand::execute()
{
    printCommandMessage(2, _client_socket, "USER " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientAuthenticatedBySocket(_client_socket) == false) // client is not authenticated
    {
        printCommandMessage(1, _client_socket, "Not authenticated");
        return;
    }

    std::vector<std::string> args = splitString(_arg, ' ');
    if (args.size() != 4) // invalid number of arguments
    {
        printCommandMessage(1, _client_socket, "Invalid number of arguments");
        return;
    }

    if (args[0].empty() || args[1].empty() || args[2].empty() || args[3].empty()) // empty parameter argument
    {
        printCommandMessage(1, _client_socket, "Cannot have empty parameter");
        return;
    }

    _client_manager.setClientNamesBySocket(_client_socket, args[0], args[1], args[2], args[3]);
    printCommandMessage(2, _client_socket, "User registered");
}

void PrivmsgCommand::execute()
{
    printCommandMessage(2, _client_socket, "PRIVMSG " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientAuthenticatedBySocket(_client_socket) == false) // client is not authenticated
    {
        printCommandMessage(1, _client_socket, "Not authenticated");
        return;
    }

    size_t pos = _arg.find(' ');
    if (pos == std::string::npos) // argument has no target or message
    {
        printCommandMessage(1, _client_socket, "Invalid number of arguments");
        return;
    }

    std::string target = _arg.substr(0, pos);
    std::string message = _arg.substr(pos + 1);
    std::string sender_nick = _client_manager.getClientNicknameBySocket(_client_socket);

    if (target[0] == '#') // channel message
    {
        if (_channel_manager.isChannelExist(target) == false) // channel does not exist
        {
            printCommandMessage(1, _client_socket, "Channel does not exist");
            return;
        }

        if (_channel_manager.isClientInChannel(target, sender_nick) == false) // client is not in channel
        {
            printCommandMessage(1, _client_socket, "Client is not in channel");
            return;
        }

        _server.sendMessageToChannel(sender_nick, target, sender_nick + " -> " + message);
        printCommandMessage(2, _client_socket, "Message sent");
    }
    else // private message
    {
        if (_client_manager.isClientExistByNick(target) == false) // target client does not exist
        {
            printCommandMessage(1, _client_socket, "Client does not exist");
            return;
        }

        if (target == sender_nick) // cannot send message to yourself
        {
            printCommandMessage(1, _client_socket, "Cannot send message to yourself");
            return;
        }

        _server.sendMessageToClientByNick(target, sender_nick + " -> " + message);
        printCommandMessage(2, _client_socket, "Message sent");
    }
}

/*  join의 고려대상 : 채널이 존재하지 않는다면 새로 생성하고, 현 유저를 오퍼레이터로 설정한다.
    채널이 존재한다면, 채널에 참여한다. 채널매니저를 통해 해당 채널의 유저목록에 현 유저를 추가한다.
    채널 모드가 invite-only라면, 참여할 수 없다.
    채널 모드가 password_set이라면, JOIN시 비밀번호를 추가로 입력해야 한다.
    채널 모드가 user_limit이라면, 채널에 참여한 유저 수가 이미 user_limit에 도달했다면 참여할 수 없다.

    1. 채널이 존재한다
    1-0. 이미 클라이언트가 서버에 존재하는가?
    1-1. 채널이 존재한다면, 해당 채널이 invite-only인가? (바로 컷)
    1-2. 채널이 존재한다면, 해당 채널이 password_set인가? (비밀번호 확인 후 컷)
    1-3. 채널이 존재한다면, 해당 채널이 user_limit인가? (채널에 참여한 유저 수 확인 후 컷)
    1-4. 채널의 유저리스트에 유저 추가

    2. 채널이 존재하지 않는다
    2-1. 채널매니저에 새로운 채널을 생성한다
    2-2. 새로운 채널의 유저리스트에 유저를 등록한다
    2-3. 새로운 채널의 운영자리스트에 유저를 등록한다.
*/
void JoinCommand::execute()
{
    printCommandMessage(2, _client_socket, "JOIN " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientAuthenticatedBySocket(_client_socket) == false) // client is not authenticated
    {
        printCommandMessage(1, _client_socket, "Not authenticated");
        return;
    }

    size_t pos = _arg.find(' ');
    std::string channel_name = "";
    std::string password = "";
    std::string client_nick = _client_manager.getClientNicknameBySocket(_client_socket);
    if (pos != std::string::npos)
    {
        channel_name = _arg.substr(0, pos);
        password = _arg.substr(pos + 1);
    }
    else
        channel_name = _arg;

    if (_channel_manager.isChannelExist(channel_name)) // channel already exist
    {
        if (_channel_manager.isClientInChannel(channel_name, client_nick)) // client already in channel
        {
            printCommandMessage(1, _client_socket, "Client already in channel");
            return;
        }

        if (_channel_manager.getChannelMode(channel_name, "I") == true)
        {
            printCommandMessage(1, _client_socket, "Channel is invite-only");
            return;
        }

        if (_channel_manager.getChannelMode(channel_name, "K") == true && _channel_manager.getChannelPassword(channel_name) != password)
        {
            printCommandMessage(1, _client_socket, "Password incorrect");
            return;
        }
        
        if (_channel_manager.getChannelMode(channel_name, "L") == true && _channel_manager.getChannelMemberList(channel_name).size() >= _channel_manager.getChannelUserLimit(channel_name))
        {
            printCommandMessage(1, _client_socket, "Channel user limit reached");
            return;
        }

        _channel_manager.addClientToChannel(channel_name, client_nick);
    }
    else // channel does not exist
    {
        _channel_manager.addChannel(channel_name, client_nick);
        printCommandMessage(2, _client_socket, "Channel " + channel_name + " created");
    }
}

void ModeCommand::execute()
{

}

void InviteCommand::execute()
{
    
}

void TopicCommand::execute()
{
    
}

void KickCommand::execute()
{
    
}