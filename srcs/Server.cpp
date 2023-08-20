#include "../includes/Server.hpp"

/**
 * @brief Server 객체 초기화 생성자
*/
Server::Server(int port, char* password)
{
    _server_port = port;
    _server_password = password;
    initServer();
    initKqueue();
}

/**
 * @brief Server 객체 소멸자
*/
Server::~Server()
{
    close(_server_socket);
}

/**
 * @brief system 함수 에러 발생
 * 
 * @param msg 출력할 에러 메시지
 * @param exit_code exit시 반환할 값
 * 
 * @details
 * 오류를 출력하고 exit code로 exit한다
*/
void Server::systemError(const char* msg, int exit_code)
{
    std::cerr << msg;
    exit(exit_code);
}

/**
 * @brief socket 생성, bind, listen
 * 
 * @details
 * socket 생성, bind, listen
 * 이때 socket을 O_NONBLOCK으로 설정하여, accept()가 blocking 되지 않도록 한다
*/
void Server::initServer()
{
    /* initialize socket */
    if ((_server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        systemError("socket() error\n", 1);
    memset(&_server_address, 0, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    _server_address.sin_port = htons(_server_port);

    /* bind socket */
    int optval = 1;
    // SO_NOSIGPIPE 사용하면 client측이 시그널로 종료되도 서버가 종료되지 않음
    if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        systemError("setsockopt() error\n",1);
    if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1)
        systemError("fcntl() error\n", 1);
    if (bind(_server_socket, (struct sockaddr*)&_server_address, sizeof(_server_address)) == -1)
        systemError("bind() error\n", 1);
    
    /* listen socket */
    if (listen(_server_socket, 3) == -1)
        systemError("listen() error\n", 1);   
}

/**
 * @brief kqueue 초기화
 * 
 * @details
 * kqueue를 초기화하고 server_socket에 대한 read event를 추가
*/
void Server::initKqueue()
{
    /* kqueue */
    if ((_kq = kqueue()) == -1)
        systemError("kqueue() error\n", 1);
    // server_socket에 대한 READ 이벤트 추가
    addEvent(_change_list, _server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

/**
 * @brief kqueue에 event 추가
*/
void Server::addEvent(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent new_event;
    EV_SET(&new_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(new_event);
}

/**
 * @brief kqueue를 통해 event를 감시, while 무한반복문을 통해 event 처리
 * 
 * @details
 * read event 발생시 fd가 server_socket인 경우, accept()를 통해 client_socket을 생성하고, client_socket에 대한 read event를 추가
 * read event 발생시 fd가 client_socket인 경우, recv()를 통해 client로부터 데이터를 받아온다
 * write event 발생시 fd가 client_socket인 경우, send()를 통해 client에게 데이터를 보낸다
*/
void Server::runServer()
{
    int             new_events;
    struct kevent*  curr_event;

    while (1)
    {
        new_events = kevent(_kq, &_change_list[0], _change_list.size(), _event_list, 16, NULL);
        if (new_events == -1)
            systemError("kevent() error\n", 1);
        _change_list.clear();

        for (int i=0; i<new_events; ++i)
        {
            curr_event = &_event_list[i];

            if (curr_event->flags == EV_ERROR)
            {
                if (curr_event->ident == static_cast<uintptr_t>(_server_socket)) // error가 server_socket에서 발생
                    systemError("server socket error in run()\n", 1);
                else
                {
                    std::cout << "client " << curr_event->ident << " error in run()\n";
                    deleteClientBySocket(curr_event->ident);
                }
            }
            else if (curr_event->filter == EVFILT_READ)
            {
                if (curr_event->ident == static_cast<uintptr_t>(_server_socket))
                {
                    int client_socket;
                    if ((client_socket = accept(_server_socket, NULL, NULL)) < 0)
                    {
                        if (errno != EWOULDBLOCK)
                            systemError("accept() error\n", 1);
                        else
                            std::cerr << "EWOULDBLOCK\n"; //should not be printed
                    }
                    else
                    {
                        if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == -1)
                            systemError("fcntl() error\n", 1);
                        addEvent(_change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                        _client_list.push_back(Client(client_socket));
                    }
                }
                else if (isClientExistBySocket(curr_event->ident))
                {
                    char buffer[256];
                    memset(buffer, 0, sizeof(buffer));
                    int n = recv(curr_event->ident, buffer, sizeof(buffer), MSG_NOSIGNAL); // MSG_NOSIGNAL : SIGPIPE 발생 방지
                    if (n >= 0)
                        buffer[n] = '\0';
                    Client& curr_client = getClientBySocket(curr_event->ident);
                    curr_client.appendReadBuffer(static_cast<std::string>(buffer));
                    int len = curr_client.getReadBuffer().length();

                    if (n < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            std::cerr << "client " << curr_event->ident << " error\n";
                            deleteClientBySocket(curr_event->ident);
                        }
                        else
                            std::cout << "EWOULDBLOCK\n"; // should not be printed
                    }
                    else if (n == 0)
                    {
                        deleteClientBySocket(curr_event->ident);
                    }
                    else if (len >= 2 && curr_client.getReadBuffer()[len - 2] == '\r' && curr_client.getReadBuffer()[len - 1] == '\n')
                    { // if input is not terminated with '\r\n', do not execute command
                        // !!! 임시
                        addEvent(_change_list, curr_event->ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
                        curr_client.setWriteBuffer(curr_client.getReadBuffer());
                        curr_client.clearReadBuffer();

                        parseAndExecuteCommand(curr_client);
                    }
                }
            }
            else if (curr_event->filter == EVFILT_WRITE) // write event
            {
                if (isClientExistBySocket(curr_event->ident))
                {
                    Client& curr_client = getClientBySocket(curr_event->ident);
                    if (curr_client.getWriteBuffer().empty())
                    {
                        // should not be printed
                        send(curr_event->ident, "Nothing to Send\n", 16, MSG_NOSIGNAL);
                    }
                    else
                    {
                        send(curr_event->ident, curr_client.getWriteBuffer().c_str(), curr_client.getWriteBuffer().length(), MSG_NOSIGNAL);
                        curr_client.clearWriteBuffer();
                        // write event가 끝났으므로, disable 및 delete 한다
                        addEvent(_change_list, curr_event->ident, EVFILT_WRITE, EV_DISABLE | EV_DELETE, 0, 0, NULL);
                    }
                }
            }
        }
    }
}

/**
 * @brief client_list에서 client를 제거
 * 
 * @param client_socket 제거할 client의 socket
 * 
 * @details 
 * 주어진 client_socket을 닫고, client_list에서 해당 client를 제거한다
*/
void Server::deleteClientBySocket(int client_socket)
{
    close(client_socket);
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
bool Server::isClientExistBySocket(int client_socket)
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
bool Server::isClientExistByNick(std::string nickname)
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
 * @warning client_list에 client_socket이 존재하는지 확인 후 사용할 것
*/
Client& Server::getClientBySocket(int client_socket)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getClientSocket() == client_socket)
            return (*it);
    }

    // must not reach
    std::cerr << "ERROR : client not found\n";
    return (*it);
}

/**
 * @brief 찾는 client를 client_list에서 찾아 반환
 * 
 * @param nickname 찾을 client의 nickname
 * 
 * @warning client_list에 nickname이 존재하는지 확인 후 사용할 것
*/
Client& Server::getClientByNick(std::string nickname)
{
    std::vector<Client>::iterator it = _client_list.begin();
    for (; it != _client_list.end(); ++it)
    {
        if (it->getNickname() == nickname)
            return (*it);
    }

    // must not reach
    std::cerr << "ERROR : client not found\n";
    return (*it);
}

/**
 * @brief 주어진 Client의 readBuffer를 파싱하여, 명령어를 실행
 * 
 * @param client 명령어를 실행할 client
*/
void Server::parseAndExecuteCommand(Client& client)
{
    
}
