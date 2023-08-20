#include "../includes/Client.hpp"

/**
 * @brief Client 객체를 초기화하는 생성자
 * 
 * @details
 * client_socket을 인자로 받아 초기화한다
 * 이후 _authenticated를 false로, 모든 string값을 빈문자열로 초기화한다
 */
Client::Client(int client_socket)
{
    _client_socket = client_socket;
    _authenticated = false;
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

