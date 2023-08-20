#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client
{
    private:

        int             _client_socket;
        std::string     _client_read_buffer;
        std::string     _client_write_buffer;

        bool            _authenticated;
        std::string     _username;
        std::string     _hostname;
        std::string     _realname;
        std::string     _servername;
        std::string     _nickname;
    
    public:

        Client(int client_socket);

        /* get methods */
        int             getClientSocket() const;
        std::string     getReadBuffer() const;
        std::string     getWriteBuffer() const;
        bool            getAuthenticated() const;
        std::string     getUsername() const;
        std::string     getHostname() const;
        std::string     getRealname() const;
        std::string     getServername() const;
        std::string     getNickname() const;

        /* set methods */
        void            setReadBuffer(std::string read_buffer);
        void            setWriteBuffer(std::string write_buffer);
        void            setAuthenticated(bool authenticated);
        void            setUsername(std::string username);
        void            setHostname(std::string hostname);
        void            setRealname(std::string realname);
        void            setServername(std::string servername);
        void            setNickname(std::string nickname);

        /* buffer */
        void            appendReadBuffer(std::string str);
        void            appendWriteBuffer(std::string str);
        void            clearReadBuffer();
        void            clearWriteBuffer();
};

#endif