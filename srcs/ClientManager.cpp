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
 * 
 * @warning channel의 client_list는 따로 제거해야 함
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
 * @return CRLF가 있음 true, 아니면 false
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
bool ClientManager::isReadBufferEndWithCRLF(int client_socket)
{
    Client& curr_client = getClientBySocket(client_socket);
    std::string read_buffer = curr_client.getReadBuffer();

    if (read_buffer.find("\r\n") != std::string::npos)
        return (true);
    return (false);
}

/**
 * @brief 특정 nick을 가진 client의 socket을 반환
 * 
 * @param nickname 찾을 client의 nickname
 * 
 * @return client의 socket
 * 
 * @warning client_list에 nickname이 존재하는지 확인 후 사용할 것
*/
int ClientManager::getClientSocketByNick(std::string nickname)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getNickname() == nickname)
            return (it->getClientSocket());
    }
    return (-1);
}

/**
 * @brief 특정 socket을 가진 client의 nickname을 반환
 * 
 * @param client_socket 찾을 client의 socket
 * 
 * @return client의 nickname
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
std::string ClientManager::getClientNicknameBySocket(int client_socket)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getClientSocket() == client_socket)
            return (it->getNickname());
    }
    return ("");
}

std::string ClientManager::getClientUsernameBySocket(int client_socket)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getClientSocket() == client_socket)
            return (it->getUsername());
    }
    return ("");
}

std::string ClientManager::getClientHostnameBySocket(int client_socket)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getClientSocket() == client_socket)
            return (it->getHostname());
    }
    return ("");
}

/**
 * @brief 특정 socket을 가진 client의 nickname을 설정
 * 
 * @param client_socket 설정할 client의 socket
 * @param nickname 설정할 nickname
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::setClientNicknameBySocket(int client_socket, std::string nickname)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.setNickname(nickname);
}

/**
 * @brief 특정 socket을 가진 client의 authenticated를 설정
 * 
 * @param client_socket 설정할 client의 socket
 * @param authenticated 설정할 authenticated
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::setClientAuthenticatedBySocket(int client_socket, bool authenticated)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.setAuthenticated(authenticated);
}

/**
 * @brief 특정 socket을 가진 client가 인증되었는지 확인
 * 
 * @param client_socket 확인할 client의 socket
 * 
 * @return 인증되었으면 true, 아니면 false
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
bool ClientManager::isClientAuthenticatedBySocket(int client_socket)
{
    Client& curr_client = getClientBySocket(client_socket);
    return (curr_client.getAuthenticated());
}

/**
 * @brief 특정 socket을 가진 client의 username, servername, hostname, realname을 설정
 * 
 * @param client_socket 설정할 client의 socket
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
void ClientManager::setClientNamesBySocket(int client_socket, std::string username, std::string servername, std::string hostname, std::string realname)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.setHostname(hostname);
    curr_client.setRealname(realname);
    curr_client.setServername(servername);
    curr_client.setUsername(username);
}

/**
 * @brief PASS, NICK, USER command가 모두 수행되었는지 확인
 * 
 * @param client_socket 확인할 client의 socket
 * 
 * @return available하면 true, 아니면 false
 * 
 * @warning client_list에 socket이 존재하는지 확인 후 사용할 것
*/
bool ClientManager::isClientReadyBySocket(int client_socket)
{
    Client& curr_client = getClientBySocket(client_socket);
    if (!curr_client.getAuthenticated() || curr_client.getNickname() == "" || curr_client.getUsername() == "" || curr_client.getHostname() == "" || curr_client.getRealname() == "" || curr_client.getServername() == "")
        return (false);
    return (true);
}

/**
 * @brief Client가 first connection sequence 중인지 확인
*/
bool ClientManager::isClientFirstConnectBySocket(int clinet_socket)
{
    Client& curr_client = getClientBySocket(clinet_socket);
    return curr_client.getFirstConnect();
}

/**
 * @brief Client가 channel에 초대됬는지 확인
*/
bool ClientManager::isClientInvitedBySocket(int client_socket, std::string channel)
{
    Client& curr_client = getClientBySocket(client_socket);
    return curr_client.isInvited(channel);
}

/**
 * @brief Client가 처음 접속한 상태인지 저장하는 flag를 주어진 값으로 수정
*/
void ClientManager::setClientFirstConnectBySocket(int client_socket, bool first_connect)
{
    Client& curr_client = getClientBySocket(client_socket);
    curr_client.setFirstConnect(first_connect);
}

/**
 * @brief Client를 channel에 초대
*/
void ClientManager::inviteClientToChannelByNick(std::string nickname, std::string channel)
{
    Client& curr_client = getClientByNick(nickname);
    curr_client.addInvite(channel);
}