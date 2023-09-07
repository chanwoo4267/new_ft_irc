#include "../includes/Client.hpp"

/**
 * @brief Client 객체를 초기화하는 생성자
 *
 * @param client_socket 클라이언트 소켓
 */
Client::Client(int client_socket)
{
    _client_socket = client_socket;
    _authenticated = false;
    _first_connect = true;
    _client_read_buffer = "";
    _client_write_buffer = "";
    _username = "";
    _hostname = "";
    _realname = "";
    _servername = "";
    _nickname = "";
}

int Client::getClientSocket() const
{
    return (_client_socket);
}

std::string Client::getReadBuffer() const
{
    return (_client_read_buffer);
}

std::string Client::getWriteBuffer() const
{
    return (_client_write_buffer);
}

bool Client::getAuthenticated() const
{
    return (_authenticated);
}

std::string Client::getUsername() const
{
    return (_username);
}

std::string Client::getHostname() const
{
    return (_hostname);
}

std::string Client::getRealname() const
{
    return (_realname);
}

std::string Client::getServername() const
{
    return (_servername);
}

std::string Client::getNickname() const
{
    return (_nickname);
}

bool Client::getFirstConnect() const
{
    return (_first_connect);
}

void Client::setReadBuffer(std::string str)
{
    _client_read_buffer = str;
}

void Client::setWriteBuffer(std::string str)
{
    _client_write_buffer = str;
}

void Client::setAuthenticated(bool authenticated)
{
    _authenticated = authenticated;
}

void Client::setUsername(std::string username)
{
    _username = username;
}

void Client::setHostname(std::string hostname)
{
    _hostname = hostname;
}

void Client::setRealname(std::string realname)
{
    _realname = realname;
}

void Client::setServername(std::string servername)
{
    _servername = servername;
}

void Client::setNickname(std::string nickname)
{
    _nickname = nickname;
}

void Client::appendReadBuffer(std::string str)
{
    _client_read_buffer += str;
}

void Client::appendWriteBuffer(std::string str)
{
    _client_write_buffer += str;
}

void Client::clearReadBuffer()
{
    _client_read_buffer.clear();
}

void Client::clearWriteBuffer()
{
    _client_write_buffer.clear();
}

/**
 * @brief invite_list에 채널이 있는지 확인한다
*/
bool Client::isInvited(std::string channel)
{
    for (std::vector<std::string>::iterator it = _invited_list.begin(); it != _invited_list.end(); it++)
    {
        if (*it == channel)
            return (true);
    }
    return (false);
}

/**
 * @brief invite_list에 채널을 추가한다
*/
void Client::addInvite(std::string channel)
{
    _invited_list.push_back(channel);
}

/**
 * @brief invite_list에서 채널을 제거한다
 * 
 * @param channel 제거할 채널
 * 
 * @note 채널이 존재하지 않으면 아무 일도 일어나지 않는다
*/
void Client::removeInvite(std::string channel)
{
    for (std::vector<std::string>::iterator it = _invited_list.begin(); it != _invited_list.end(); it++)
    {
        if (*it == channel)
        {
            _invited_list.erase(it);
            return ;
        }
    }
}
