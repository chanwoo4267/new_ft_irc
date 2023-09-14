#include "../includes/Command.hpp"

void CapCommand::execute()
{
    if (_arg.find("JOIN") != std::string::npos)
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 451 * JOIN :You have not registered");
}

void PassCommand::execute()
{
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
        return;
    }

    if (_client_manager.isClientExistByNick(_arg)) // duplicated nickname
    {
        if (_client_manager.isClientFirstConnectBySocket(_client_socket)) // first connect, but duplicated nick
        {
            _server.sendMessageToClientBySocket(_client_socket, ":irc.local 433 * " + _arg + " :Nickname is already in use.");
            while (_client_manager.isClientExistByNick(_arg))
                _arg += "_";
            _client_manager.setClientNicknameBySocket(_client_socket, _arg);
            _server.sendMessageToClientBySocket(_client_socket, ":" + _arg + "!@" + "127.0.0.1" + " NICK :" + _arg); // username 없음
        }
        return;
    }

    std::string nick = _client_manager.getClientNicknameBySocket(_client_socket);
    _client_manager.setClientNicknameBySocket(_client_socket, _arg);
    std::string username = _client_manager.getClientUsernameBySocket(_client_socket);
    std::string hostname = _client_manager.getClientHostnameBySocket(_client_socket);
    if (hostname.empty())
        hostname = "127.0.0.1";
    _server.sendMessageToClientBySocket(_client_socket, ":" + nick + "!" + username + "@" + hostname + " NICK :" + _arg);
}

void UserCommand::execute()
{
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
    if (pos == std::string::npos)
        return;
    std::string username = _arg.substr(0, pos);
    _arg = _arg.substr(pos + 1);

    pos = _arg.find(' ');
    if (pos == std::string::npos)
        return;
    std::string servername = _arg.substr(0, pos);
    _arg = _arg.substr(pos + 1);

    pos = _arg.find(' ');
    if (pos == std::string::npos)
        return;
    std::string hostname = _arg.substr(0, pos);

    if (_arg[pos + 1] != ':') // invalid realname format
        return;
    _arg = _arg.substr(pos + 2);

    _client_manager.setClientNamesBySocket(_client_socket, username, servername, hostname, _arg);
    printCommandMessage(2, _client_socket, "User info confirmed");

    // success message send
    std::string nick = _client_manager.getClientNicknameBySocket(_client_socket);
    if (_client_manager.isClientFirstConnectBySocket(_client_socket))
    {
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + username + "@" + hostname);
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 002 " + nick + " :Your host is " + hostname + " running version 1.0");
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 003 " + nick + " :This server was created sometime");
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 004 " + nick + " :" + hostname + " 1.0 Channel modes +ntikl");
        _client_manager.setClientFirstConnectBySocket(_client_socket, false); // following USER command does not need to send welcome message
    }
}

void PrivmsgCommand::execute()
{
    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientReadyBySocket(_client_socket) == false) // client is not authenticated
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
            _server.sendMessageToClientBySocket(_client_socket, ":irc.local 401 " + sender_nick + " " + target + " :No such channel");
            return;
        }

        if (_channel_manager.isClientInChannel(target, sender_nick) == false) // client is not in channel
        {
            _server.sendMessageToClientBySocket(_client_socket, ":irc.local 404 " + sender_nick + " " + target + " :You cannot send external messages to this channel whilst the +n (noextmsg) mode is set.");
            return;
        }

        // 입력된 message에 : 이 포함되므로 여기서는 따로 추가해주지 않는다
        _server.sendMessageToChannelExceptMe(sender_nick, target, ":" + sender_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " PRIVMSG " + target + " " + message);
    }
    else // private message
    {
        if (_client_manager.isClientExistByNick(target) == false) // target client does not exist
        {
            printCommandMessage(1, _client_socket, "Client does not exist");
            _server.sendMessageToClientBySocket(_client_socket, ":irc.local 401 " + sender_nick + " " + target + " :No such nick");
            return;
        }

        // 자신에게 메시지를 보내도 상관없음
        // 입력된 message에 : 이 포함되므로 여기서는 따로 추가해주지 않는다
        _server.sendMessageToClientByNick(target, ":" + sender_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " PRIVMSG " + target + " " + message);
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
    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientReadyBySocket(_client_socket) == false) // client is not authenticated
    {
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 451 * JOIN :You have not registered");
        return;
    }

    /* channel_name : 채널 이름 , password : 채널 비밀번호, client_nick : JOIN한 유저의 닉네임 */
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
        // 이미 존재하는 채널에 다시 JOIN 하는건 irssi 측에서 걸러줌

        // channel is invite-only, and joiner is not invited
        if (_channel_manager.getChannelMode(channel_name, "I") == true && _client_manager.isClientInvitedBySocket(_client_socket, channel_name) == false)
        {
            _server.sendMessageToClientBySocket(_client_socket, ":irc.local 473 " + client_nick + " " + channel_name + " :Cannot join channel (invite only)");
            return;
        }

        if (_channel_manager.getChannelMode(channel_name, "K") == true && _channel_manager.getChannelPassword(channel_name) != password)
        {
            _server.sendMessageToClientBySocket(_client_socket, ":irc.local 475 " + client_nick + " " + channel_name + " :Cannot join channel (incorrect channel key)");
            return;
        }
        
        if (_channel_manager.getChannelMode(channel_name, "L") == true && _channel_manager.getChannelMemberList(channel_name).size() >= _channel_manager.getChannelUserLimit(channel_name))
        {
            _server.sendMessageToClientBySocket(_client_socket, ":irc.local 471 " + client_nick + " " + channel_name + " :Cannot join channel (channel is full)");
            return;
        }

        _channel_manager.addClientToChannel(channel_name, client_nick);

        // announce joining to all users
        std::string formatted_user = ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket);
        _server.sendMessageToChannel(client_nick, channel_name, formatted_user + " JOIN :" + channel_name);

        // send user list to joiner
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 353 " + client_nick + " = " + channel_name + " :" + _channel_manager.getChannelMemberListString(channel_name));
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 366 " + client_nick + " " + channel_name + " :End of /NAMES list.");
    }
    else // channel does not exist
    {
        _channel_manager.addChannel(channel_name, client_nick);

        // announce joining to all users
        std::string formatted_user = ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket);
        _server.sendMessageToChannel(client_nick, channel_name, formatted_user + " JOIN " + channel_name);

        // send user list to joiner
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 353 " + client_nick + " = " + channel_name + " :@" + client_nick);
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 366 " + client_nick + " " + channel_name + " :End of /NAMES list.");
    }
}

void ModeCommand::execute()
{
    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientReadyBySocket(_client_socket) == false) // client is not authenticated
    {
        printCommandMessage(1, _client_socket, "Not authenticated");
        return;
    }

    // parsing arguments
    size_t pos = _arg.find(' ');
    std::string channel_name = "";
    std::string mode = "";
    std::string value = "";
    std::string client_nick = _client_manager.getClientNicknameBySocket(_client_socket);

    if (pos == std::string::npos)
    {
        printCommandMessage(1, _client_socket, "Invalid number of arguments");
        return;
    }

    channel_name = _arg.substr(0, pos);
    mode = _arg.substr(pos + 1);
    pos = mode.find(' ');
    if (pos != std::string::npos)
    {
        value = mode.substr(pos + 1);
        mode = mode.substr(0, pos);
    }
    // parsing end

    if (_channel_manager.isChannelExist(channel_name) == false) // channel does not exist, or [mode user +i] 도 여기서 걸러주게됨
        return;

    if (_channel_manager.isClientOperatorInChannel(channel_name, client_nick) == false) // client is not operator of channel
    {
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 482 " + client_nick + " " + channel_name + " :You're not channel operator");
        return;
    }

    if (mode[0] != '+' && mode[0] != '-')
    {
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 502 " + client_nick + " :Can't view modes for other users");
        return;
    }

    if (mode.length() != 2 || (mode[1] != 'i' && mode[1] != 't' && mode[1] != 'o' && mode[1] != 'k' && mode[1] != 'l')) // invalid mode input
    {
        _server.sendMessageToClientBySocket(_client_socket, ":irc.local 472 " + client_nick + " :is not a recognised channel mode.");
        return;
    }

    if (mode[1] == 'i' || mode[1] == 'I')
    {
        _channel_manager.setChannelMode(channel_name, "I", mode[0] == '+');
        _server.sendMessageToChannel(client_nick, channel_name, ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " MODE " + channel_name + " :" + mode);
    }
    else if (mode[1] == 't' || mode[1] == 'T')
    {
        _channel_manager.setChannelMode(channel_name, "T", mode[0] == '+');
        _server.sendMessageToChannel(client_nick, channel_name, ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " MODE " + channel_name + " :" + mode);
    }
    else if (mode[1] == 'k' || mode[1] == 'K')
    {
        if (mode[0] == '+') // set channel password
        {
            if (value == "") // no password input
            {
                _server.sendMessageToClientBySocket(_client_socket, ":irc.local 696 " + client_nick + " " + channel_name + " k * :You must specify a parameter for the key mode. Syntax: <key>.");
                return;
            }
            _channel_manager.setChannelMode(channel_name, "K", true);
            _channel_manager.setChannelPassword(channel_name, value);
            _server.sendMessageToChannel(client_nick, channel_name, ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " MODE " + channel_name + " " + mode + " :" + value);
        }
        else // remove channel password
        {
            std::string temp_pw = _channel_manager.getChannelPassword(channel_name);
            _channel_manager.setChannelMode(channel_name, "K", false);
            _channel_manager.setChannelPassword(channel_name, "");
            _server.sendMessageToChannel(client_nick, channel_name, ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " MODE " + channel_name + " " + mode + " :" + temp_pw);
        }
    }
    else if (mode[1] == 'l' || mode[1] == 'L')
    {
        if (mode[0] == '+')
        {
            if (value == "")
            {
                _server.sendMessageToClientBySocket(_client_socket, ":irc.local 696 " + client_nick + " " + channel_name + " l * :You must specify a parameter for the limit mode. Syntax: <limit>.");
                return;
            }
            _channel_manager.setChannelMode(channel_name, "L", true);
            _channel_manager.setChannelUserLimit(channel_name, atoi(value.c_str()));
            _server.sendMessageToChannel(client_nick, channel_name, ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " MODE " + channel_name + " " + mode + " :" + value);
        }
        else
        {
            _channel_manager.setChannelMode(channel_name, "L", false);
            _channel_manager.setChannelUserLimit(channel_name, 0);
            _server.sendMessageToChannel(client_nick, channel_name, ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " MODE " + channel_name + " :" + mode);
        }
    }
    else if (mode[1] == 'o' || mode[1] == 'O')
    {

        if (_channel_manager.isClientInChannel(channel_name, value) == false) // client is not in channel
        {
            _server.sendMessageToClientBySocket(_client_socket, ":irc.local 401 " + client_nick + " " + value + " :No such nick");
            return;
        }

        if (mode[0] == '+') // give oper privilege
        {
            if (value == "")
            {
                _server.sendMessageToClientBySocket(_client_socket, ":irc.local 696 " + client_nick + " " + channel_name + " o * :You must specify a parameter for the operator mode. Syntax: <nick>.");
                return;
            }

            if (_channel_manager.isClientOperatorInChannel(channel_name, value)) // client is already operator of channel
                return;

            _channel_manager.addOperToChannel(channel_name, value);
            _server.sendMessageToChannel(client_nick, channel_name, ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " MODE " + channel_name + " " + mode + " :" + value);
        }
        else // remove oper privilege
        {
            if (value == "")
            {
                _server.sendMessageToClientBySocket(_client_socket, ":irc.local 696 " + client_nick + " " + channel_name + " o * :You must specify a parameter for the operator mode. Syntax: <nick>.");
                return;
            }

            if (_channel_manager.isClientOperatorInChannel(channel_name, value) == false) // client is not operator of channel
                return;

            if (value == client_nick) // cannot remove yourself from operator list, need to check
            {
                printCommandMessage(1, _client_socket, "Cannot remove yourself from operator list");
                return;
            }

            _channel_manager.deleteOperFromChannel(channel_name, value);
            _server.sendMessageToChannel(client_nick, channel_name, ":" + client_nick + "!" + _client_manager.getClientUsernameBySocket(_client_socket) + "@" + _client_manager.getClientHostnameBySocket(_client_socket) + " MODE " + channel_name + " " + mode + " :" + value);
        }
    }
}

void InviteCommand::execute()
{
    printCommandMessage(2, _client_socket, "INVITE " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientReadyBySocket(_client_socket) == false) // client is not authenticated
    {
        printCommandMessage(1, _client_socket, "Not authenticated");
        return;
    }

    size_t pos = _arg.find(' ');
    std::string invited = "";
    std::string channel_name = "";
    std::string invitor = _client_manager.getClientNicknameBySocket(_client_socket);

    if (pos == std::string::npos)
    {
        printCommandMessage(1, _client_socket, "Invalid number of arguments");
        return;
    }
    invited = _arg.substr(0, pos);
    channel_name = _arg.substr(pos + 1);

    if (_channel_manager.isChannelExist(channel_name) == false) // channel does not exist
    {
        printCommandMessage(1, _client_socket, "Channel does not exist");
        return;
    }

    if (_channel_manager.isClientInChannel(channel_name, invitor) == false) // invitor is not in channel
    {
        printCommandMessage(1, _client_socket, "Invitor is not in channel");
        return;
    }

    // channel is invite-only, then only operator can invite
    if (_channel_manager.getChannelMode(channel_name, "I") && _channel_manager.isClientOperatorInChannel(channel_name, invitor) == false)
    {
        printCommandMessage(1, _client_socket, "Channel is Invite-only, Invitor is not operator of channel");
        return;
    }

    if (_client_manager.isClientExistByNick(invited) == false) // invited client does not exist
    {
        printCommandMessage(1, _client_socket, "Invited client does not exist");
        return;
    }

    if (_channel_manager.isClientInChannel(channel_name, invited)) // invited client is already in channel
    {
        printCommandMessage(1, _client_socket, "Invited client is already in channel");
        return;
    }

    _channel_manager.addClientToChannel(channel_name, invited);
    printCommandMessage(2, _client_socket, "Client " + invited + " added to channel " + channel_name);
}

void TopicCommand::execute()
{
    printCommandMessage(2, _client_socket, "TOPIC " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientReadyBySocket(_client_socket) == false) // client is not authenticated
    {
        printCommandMessage(1, _client_socket, "Not authenticated");
        return;
    }

    size_t pos = _arg.find(' ');
    std::string channel_name = "";
    std::string topic = "";
    std::string client_nick = _client_manager.getClientNicknameBySocket(_client_socket);

    if (pos == std::string::npos)
        channel_name = _arg;
    else
    {
        channel_name = _arg.substr(0, pos);
        topic = _arg.substr(pos + 1);
    }

    if (_channel_manager.isChannelExist(channel_name) == false) // channel does not exist
    {
        printCommandMessage(1, _client_socket, "Channel does not exist");
        return;
    }

    if (_channel_manager.isClientInChannel(channel_name, client_nick) == false) // client is not in channel
    {
        printCommandMessage(1, _client_socket, "Client is not in channel");
        return;
    }

    if (topic == "") // get topic
    {
        _server.sendMessageToClientByNick(client_nick, _channel_manager.getChannelTopic(channel_name));
        printCommandMessage(2, _client_socket, "Topic printed");
    }
    else // set topic
    {
        // channel is Topic_limit, so oper can only change
        if (_channel_manager.getChannelMode(channel_name, "T") && _channel_manager.isClientOperatorInChannel(channel_name, client_nick) == false)
        {
            printCommandMessage(1, _client_socket, "Client is not operator of channel");
            return;
        }

        _channel_manager.setChannelTopic(channel_name, topic);
        printCommandMessage(2, _client_socket, "Topic set to " + topic);
    }
}

void KickCommand::execute()
{
    printCommandMessage(2, _client_socket, "KICK " + _arg);

    if (_client_manager.isClientExistBySocket(_client_socket) == false) // client is not connected to server
    {
        printCommandMessage(1, _client_socket, "Not connected to server");
        return;
    }

    if (_client_manager.isClientReadyBySocket(_client_socket) == false) // client is not authenticated
    {
        printCommandMessage(1, _client_socket, "Not authenticated");
        return;
    }

    size_t pos = _arg.find(' ');
    std::string channel_name = "";
    std::string kicked = "";
    std::string client_nick = _client_manager.getClientNicknameBySocket(_client_socket);

    if (pos == std::string::npos)
    {
        printCommandMessage(1, _client_socket, "Invalid number of arguments");
        return;
    }

    channel_name = _arg.substr(0, pos);
    kicked = _arg.substr(pos + 1);

    if (_channel_manager.isChannelExist(channel_name) == false) // channel does not exist
    {
        printCommandMessage(1, _client_socket, "Channel does not exist");
        return;
    }

    if (_channel_manager.isClientInChannel(channel_name, client_nick) == false) // client is not in channel
    {
        printCommandMessage(1, _client_socket, "Client is not in channel");
        return;
    }

    // kick is only for operator
    if (_channel_manager.isClientOperatorInChannel(channel_name, client_nick) == false) // client is not operator of channel
    {
        printCommandMessage(1, _client_socket, "Client is not operator of channel");
        return;
    }

    if (_channel_manager.isClientInChannel(channel_name, kicked) == false) // kicked client is not in channel
    {
        printCommandMessage(1, _client_socket, "Kicked client is not in channel");
        return;
    }

    if (kicked == client_nick) // cannot kick yourself
    {
        printCommandMessage(1, _client_socket, "Cannot kick yourself");
        return;
    }

    _channel_manager.deleteClientFromChannel(channel_name, kicked);
    if (_channel_manager.isClientOperatorInChannel(channel_name, kicked))
        _channel_manager.deleteOperFromChannel(channel_name, kicked);
    printCommandMessage(2, _client_socket, "Client " + kicked + " kicked from channel " + channel_name);
}

/**
 * @brief send PING to client
*/
void PingCommand::execute()
{
    std::string hostname = _client_manager.getClientHostnameBySocket(_client_socket);
    _server.sendMessageToClientBySocket(_client_socket, "PING :irc.local");
}

/**
 * @brief send PONG to client
*/
void PongCommand::execute()
{
    std::string hostname = _client_manager.getClientHostnameBySocket(_client_socket);
    _server.sendMessageToClientBySocket(_client_socket, "PONG irc.local :irc.local");
}