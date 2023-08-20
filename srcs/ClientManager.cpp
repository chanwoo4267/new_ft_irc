#include "../includes/ClientManager.hpp"

/**
 * @brief client_list에 client를 추가
 * 
 * @param client_socket 추가할 client의 socket
*/
void ClientManager::addClient(int client_socket)
{
    Client new_client(client_socket);
    _client_list.push_back(new_client);
}

/**
 * @brief client_list에서 client를 제거
 * 
 * @param client_socket 제거할 client의 socket
*/
void ClientManager::deleteClientBySocket(int client_socket)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getClientSocket() == client_socket)
        {
            _client_list.erase(it);
            break;
        }
    }
}

/**
 * @brief client_list에서 client를 제거
 * 
 * @param nickname 제거할 client의 nickname
*/
void ClientManager::deleteClientByNick(std::string nickname)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getNickname() == nickname)
        {
            _client_list.erase(it);
            break;
        }
    }
}

/**
 * @brief client_list에 client가 존재하는지 확인
 * 
 * @param client_socket 확인할 client의 socket
*/
bool ClientManager::isClientExistBySocket(int client_socket)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getClientSocket() == client_socket)
            return (true);
    }
    return (false);
}

/**
 * @brief client_list에 client가 존재하는지 확인
 * 
 * @param nickname 확인할 client의 nickname
*/
bool ClientManager::isClientExistByNick(std::string nickname)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getNickname() == nickname)
            return (true);
    }
    return (false);
}

/**
 * @brief 찾는 client를 client_list에서 찾아 반환
 * 
 * @param client_socket 찾을 client의 socket
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
Client& ClientManager::getClientBySocket(int client_socket)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getClientSocket() == client_socket)
            return (*it);
    }
    return (*it);
}

/**
 * @brief 찾는 client를 client_list에서 찾아 반환
 * 
 * @param nickname 찾을 client의 nickname
 * 
 * @warning client_list에 nickname이 존재하는지 확인 후 사용할 것
*/
Client& ClientManager::getClientByNick(std::string nickname)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getNickname() == nickname)
            return (*it);
    }
    return (*it);
}

/**
 * @brief 특정 client의 read_buffer에 문자열을 추가
 * 
 * @param client_socket read_buffer에 문자열을 추가할 client의 socket
 * @param str 추가할 문자열
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::appendReadBufferBySocket(int client_socket, std::string str)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.appendReadBuffer(str);
}

/**
 * @brief 특정 client의 write_buffer에 문자열을 추가
 * 
 * @param client_socket write_buffer에 문자열을 추가할 client의 socket
 * @param str 추가할 문자열
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::appendWriteBufferBySocket(int client_socket, std::string str)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.appendWriteBuffer(str);
}

/**
 * @brief 특정 client의 read_buffer를 비움
 * 
 * @param client_socket read_buffer를 비울 client의 socket
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::clearReadBufferBySocket(int client_socket)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.clearReadBuffer();
}

/**
 * @brief 특정 client의 write_buffer를 비움
 * 
 * @param client_socket write_buffer를 비울 client의 socket
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::clearWriteBufferBySocket(int client_socket)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.clearWriteBuffer();
}

/**
 * @brief 특정 client의 read_buffer를 설정
 * 
 * @param client_socket read_buffer를 설정할 client의 socket
 * @param str 설정할 문자열
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::setReadBufferBySocket(int client_socket, std::string str)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.setReadBuffer(str);
}

/**
 * @brief 특정 client의 write_buffer를 설정
 * 
 * @param client_socket write_buffer를 설정할 client의 socket
 * @param str 설정할 문자열
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::setWriteBufferBySocket(int client_socket, std::string str)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.setWriteBuffer(str);
}

/**
 * @brief 특정 client의 read_buffer를 반환
 * 
 * @param client_socket read_buffer를 반환할 client의 socket
 * 
 * @return client의 read buffer
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
std::string ClientManager::getReadBufferBySocket(int client_socket)
{
    Client& curr_client = getClientBySocket(client_socket);
    return (curr_client.getReadBuffer());
}

/**
 * @brief 특정 client의 write_buffer를 반환
 * 
 * @param client_socket write_buffer를 반환할 client의 socket
 * 
 * @return client의 write buffer
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
std::string ClientManager::getWriteBufferBySocket(int client_socket)
{
    Client& curr_client = getClientBySocket(client_socket);
    return (curr_client.getWriteBuffer());
}

/**
 * @brief 특정 client의 read_buffer가 CRLF로 끝나는지 확인
 * 
 * @param client_socket 확인할 client의 socket
 * 
 * @return CRLF로 끝나면 true, 아니면 false
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
bool ClientManager::isReadBufferEndWithCRLF(int client_socket)
{
    Client& curr_client = getClientBySocket(client_socket);
    std::string read_buffer = curr_client.getReadBuffer();
    int len = read_buffer.length();
    if (len >= 2 && read_buffer[len - 2] == '\r' && read_buffer[len - 1] == '\n')
        return (true);
    return (false);
}