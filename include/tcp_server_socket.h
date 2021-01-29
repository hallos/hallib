#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#ifndef WIN32
#define INVALID_SOCKET -1
#endif

#include <memory>
#include <exception>
#include "tcp_stream_socket.h"

typedef int Socket;

class tcp_socket_exception : public std::exception
{
public:
    tcp_socket_exception(const std::string what): what_(what) {};
    virtual ~tcp_socket_exception() {};
    const std::string what() { return what_; }
private:
    std::string what_;
};

class Itcp_server_socket
{
public:
    virtual ~Itcp_server_socket() {};

    virtual std::unique_ptr<Itcp_stream_socket> acceptConnection() = 0;
};

class tcp_server_socket : public Itcp_server_socket
{
public:
    tcp_server_socket(int port);
    virtual ~tcp_server_socket();

    std::unique_ptr<Itcp_stream_socket> acceptConnection();
private:
    int port_;
    Socket socket_;

    void closeSocket();
};

#endif 