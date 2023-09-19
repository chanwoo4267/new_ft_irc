#include "../includes/Server.hpp"

/**
 * @brief Server 객체 초기화 생성자
 * 
 * @param port 서버 포트
 * @param password 서버 비밀번호
*/
Server::Server(int port, std::string password)
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
*/
void Server::systemError(const char* msg, int exit_code)
{
    printServerMessage(0, msg);
    exit(exit_code);
}

/**
 * @brief socket 생성, bind, listen
 * 
 * @note socket을 O_NONBLOCK으로 설정하여, accept()가 blocking 되지 않도록 한다
*/
void Server::initServer()
{
    /* initialize socket */
    if ((_server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        systemError("socket() error", 1);
    memset(&_server_address, 0, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    _server_address.sin_port = htons(_server_port);

    /* bind socket */
    int optval = 1;
    // SO_NOSIGPIPE 사용하면 client측이 시그널로 종료되도 서버가 종료되지 않음
    if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        systemError("setsockopt() error",1);
    if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1)
        systemError("fcntl() error", 1);
    if (bind(_server_socket, (struct sockaddr*)&_server_address, sizeof(_server_address)) == -1)
        systemError("bind() error", 1);
    
    /* listen socket */
    if (listen(_server_socket, 3) == -1)
        systemError("listen() error", 1);   
}

/**
 * @brief kqueue 초기화 및 read event 추가
*/
void Server::initKqueue()
{
    /* kqueue */
    if ((_kq = kqueue()) == -1)
        systemError("kqueue() error", 1);
    // server_socket에 대한 READ 이벤트 추가
    addEvent(_change_list, _server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

/**
 * @brief kqueue에 주어진 event 추가
*/
void Server::addEvent(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent new_event;
    EV_SET(&new_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(new_event);
}

/**
 * @brief kqueue에 주어진 message 출력 event 추가
 * 
 * @param ident event를 추가할 fd
 * @param message 출력할 메시지
 * 
 * @note
 * 특정 client에 메시지를 보내기 위해 사용
*/
void Server::addPrintEvent(uintptr_t ident, std::string message)
{
    _client_manager.appendWriteBufferBySocket(ident, message);
    addEvent(_change_list, ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);

    //debug
    printServerMessage(2, "client " + std::to_string(ident) + " <- " + message);
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
            systemError("kevent() error", 1);
        _change_list.clear();

        // if (_client_manager.isClientExistBySocket(5))
        //     printServerMessage(1, "client 5 : " + _client_manager.getReadBufferBySocket(5));

        for (int i=0; i<new_events; ++i)
        {
            curr_event = &_event_list[i];

            if (curr_event->flags == EV_ERROR)
            {
                if (curr_event->ident == static_cast<uintptr_t>(_server_socket)) // error가 server_socket에서 발생
                    systemError("server socket error in run()", 1);
                else
                {
                    printServerMessage(0, "client " + std::to_string(curr_event->ident) + " error in run()");
                    close(curr_event->ident);
                    // channel list에서 client 관련정보 삭제
                    _channel_manager.deleteClientFromAllChannel(_client_manager.getClientNicknameBySocket(curr_event->ident));
                    // client list에서 client 삭제
                    _client_manager.deleteClientBySocket(curr_event->ident);
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
                            systemError("accept() error", 1);
                        else
                            printServerMessage(1, "EWOULDBLOCK");
                    }
                    else
                    {
                        if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == -1)
                            systemError("fcntl() error", 1);
                        addEvent(_change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                        _client_manager.addClient(client_socket);
                        printServerMessage(2, "client " + std::to_string(client_socket) + " connected");
                    }
                }
                else if (_client_manager.isClientExistBySocket(curr_event->ident))
                {
                    char buffer[256];
                    memset(buffer, 0, sizeof(buffer));
                    int n = recv(curr_event->ident, buffer, sizeof(buffer), MSG_NOSIGNAL); // MSG_NOSIGNAL : SIGPIPE 발생 방지
                    if (n >= 0)
                        buffer[n] = '\0';

                    _client_manager.appendReadBufferBySocket(curr_event->ident, static_cast<std::string>(buffer));

                    if (n < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            printServerMessage(1, "client " + std::to_string(curr_event->ident) + " unexpected error, so disconnect");
                            close(curr_event->ident);
                            _channel_manager.deleteClientFromAllChannel(_client_manager.getClientNicknameBySocket(curr_event->ident));
                            _client_manager.deleteClientBySocket(curr_event->ident);
                        }
                        else
                            printServerMessage(1, "EWOULDBLOCK");
                    }
                    else if (n == 0) // nc에서 cltr+c, cltr+d 로 접속종료
                    {
                        printServerMessage(2, "client " + std::to_string(curr_event->ident) + " disconnected");
                        close(curr_event->ident);
                        std::string del_nick = _client_manager.getClientNicknameBySocket(curr_event->ident);
                        _channel_manager.deleteClientFromAllChannel(del_nick); // channel list에서 해당 클라이언트 삭제
                        _client_manager.deleteClientBySocket(curr_event->ident); // client list에서 삭제
                    }
                    else if (_client_manager.isReadBufferEndWithCRLF(curr_event->ident)) // if input is terminated with '\r\n', execute command
                    {
                        parseAndExecuteCommand(curr_event->ident);
                    }
                }
            }
            else if (curr_event->filter == EVFILT_WRITE) // write event
            {
                if (_client_manager.isClientExistBySocket(curr_event->ident))
                {
                    if (_client_manager.getWriteBufferBySocket(curr_event->ident).empty())
                    {
                        // should not be printed
                        send(curr_event->ident, "Nothing to Send\n", 16, MSG_NOSIGNAL);
                    }
                    else
                    {
                        std::string read_string = _client_manager.getWriteBufferBySocket(curr_event->ident);
                        while (read_string.find("\r\n") != std::string::npos)
                        {
                            std::string send_msg = read_string.substr(0, read_string.find("\r\n") + 2);
                            read_string = read_string.substr(read_string.find("\r\n") + 2);
                            send(curr_event->ident, send_msg.c_str(), send_msg.length(), MSG_NOSIGNAL);
                        }
                        _client_manager.setWriteBufferBySocket(curr_event->ident, read_string);
                        // write event가 끝났으므로, disable 및 delete 한다
                        addEvent(_change_list, curr_event->ident, EVFILT_WRITE, EV_DISABLE | EV_DELETE, 0, 0, NULL);
                    }
                }
            }
        }
    }
}

/**
 * @brief 주어진 command와 argument를 통해 Command 객체 생성
 * 
 * @param client_socket 명령어를 실행할 client의 socket
 * @param command 명령어
 * @param args 명령어 인자
 * 
 * @return Command 객체
 * 
 * @note
 * 주어진 명령어가 존재하지 않는 경우, NULL을 반환한다
*/
Command* Server::createCommand(int client_socket, std::string command, std::string arg)
{
    if (command == "pass")
        return new PassCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "cap")
        return new CapCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "nick")
        return new NickCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "user" || command == "userhost")
        return new UserCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "privmsg")
        return new PrivmsgCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "join")
        return new JoinCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "mode")
        return new ModeCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "invite")
        return new InviteCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "topic")
        return new TopicCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "kick")
        return new KickCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "ping") // if client send PING
        return new PongCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else if (command == "refresh")
        return new PingCommand(client_socket, *this, _client_manager, _channel_manager, arg);
    else
        return NULL;
}

/**
 * @brief 주어진 Client의 readBuffer를 받아와 명령어를 분리하고, 이에 맞는 명령을 실행
 *
 * @param client_socket 명령어를 실행할 client의 socket
 *
 * @note
 * 명령어가 존재하지 않는 경우, 서버에 오류 메시지 출력
 */
void Server::parseAndExecuteCommand(int client_socket)
{
    std::vector<std::string> args;
    std::string read_string;
    std::string read_buffer;
    std::string command = "";
    std::string argument = "";
    size_t pos = 0;

    // read from client buffer
    read_string = _client_manager.getReadBufferBySocket(client_socket);

    while (read_string.find("\r\n") != std::string::npos) // \r\n 이 없을때까지 명령 반복수행
    {
        read_buffer = read_string.substr(0, read_string.find("\r\n"));
        read_string = read_string.substr(read_string.find("\r\n") + 2);

        // debug
        printServerMessage(2, "client " + std::to_string(client_socket) + " -> " + read_buffer);

        // parse command
        if ((pos = read_buffer.find(' ')) != std::string::npos)
        {
            command = read_buffer.substr(0, pos);
            argument = read_buffer.substr(pos + 1);
        }
        else
            command = read_buffer;

        // command to lower case
        for (std::string::iterator it = command.begin(); it != command.end(); ++it)
            *it = std::tolower(*it);

        Command *cmd = createCommand(client_socket, command, argument);
        if (cmd)
        {
            cmd->execute();
            delete cmd;
        }
    }

    _client_manager.setReadBufferBySocket(client_socket, read_string);

    // debug
    // printServerMessage(2, "client " + std::to_string(client_socket) + " BUFFER : " + _client_manager.getReadBufferBySocket(client_socket));
}

/**
 * @brief 주어진 nick을 가진 client에게 message를 보낸다
 * 
 * @param nick 메시지를 보낼 client의 nick
 * @param message 보낼 메시지
 * 
 * @note
 * 주어진 nick의 클라이언트가 존재하지 않는 경우, 서버에 오류 메시지 출력
*/
void Server::sendMessageToClientByNick(std::string nick, std::string message)
{
    if (_client_manager.isClientExistByNick(nick))
    {
        int socket = _client_manager.getClientSocketByNick(nick);
        addPrintEvent(socket, message + "\r\n"); // 출력 메시지 가독성을 위해 개행 추가
        addPrintEvent(socket, "PING irc.local :irc.local\r\n");
    }
}

/**
 * @brief 주어진 socket을 가진 client에게 message를 보낸다
 * 
 * @param socket 메시지를 보낼 client의 socket
 * @param message 보낼 메시지
 * 
 * @note 해당 client가 없으면 아무일도 없음
 * @note 보내줄 때 자동으로 \r\n을 붙여서 보내줌
*/
void Server::sendMessageToClientBySocket(int socket, std::string message)
{
    if (_client_manager.isClientExistBySocket(socket))
    {
        addPrintEvent(socket, message + "\r\n");
        addPrintEvent(socket, "PING irc.local :irc.local\r\n");
    }
}

/**
 * @brief 주어진 channel에게 message를 자신을 포함한 모든 client에게 보낸다
 * 
 * @param nick 메시지를 보낸 client의 nick
 * @param channel 메시지를 보낼 channel
 * @param message 보낼 메시지
 * 
 * @warning channel이 존재하는지 확인 후 사용
*/
void Server::sendMessageToChannel(std::string nick, std::string channel, std::string message)
{
    std::vector<std::string> member_list = _channel_manager.getChannelMemberList(channel);
    for (std::vector<std::string>::iterator it = member_list.begin(); it != member_list.end(); ++it)
    {
        sendMessageToClientByNick(*it, message);
    }
}

/**
 * @brief 주어진 channel에게 message를 자신을 제외한 모든 client에게 보낸다
*/
void Server::sendMessageToChannelExceptMe(std::string nick, std::string channel, std::string message)
{
    std::vector<std::string> member_list = _channel_manager.getChannelMemberList(channel);
    for (std::vector<std::string>::iterator it = member_list.begin(); it != member_list.end(); ++it)
    {
        if (*it != nick)
            sendMessageToClientByNick(*it, message);
    }
}

std::string Server::getPassword() const
{
    return _server_password;
}
