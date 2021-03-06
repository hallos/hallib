#include "tcp_server_socket.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> //close
#endif

namespace hallos
{

/**
 * Constructor
 */ 
tcp_server_socket::tcp_server_socket(int port)
{
#if WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0)
    {
        throw tcp_socket_exception("Failed to startup winsock.");
    }
    if (!(LOBYTE(wsaData.wVersion) >= 2))
    {
        throw tcp_socket_exception("Too old winsock version.");
    }
#endif //WIN32

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET)
    {
        throw tcp_socket_exception("Could not create socket.");
    } 

    linger so_linger = {1, 0};
    setsockopt(socket_, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

    sockaddr_in sockAdr = {0};
    sockAdr.sin_family = AF_INET;
    sockAdr.sin_port = htons(port);  
    sockAdr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_, reinterpret_cast<sockaddr*>(&sockAdr), sizeof(sockAdr)) != 0)
    {
        closeSocket();
        throw tcp_socket_exception("Could not bind socket.");
    }

    if (listen(socket_, SOMAXCONN)!=0)
    {
        closeSocket();
        throw tcp_socket_exception("Could not set socket in listening mode.");
    }   
}

/**
 * Destructor
 */ 
tcp_server_socket::~tcp_server_socket()
{
    closeSocket();
}

/**
 * 
 */
std::unique_ptr<Itcp_stream_socket> tcp_server_socket::acceptConnection()
{
    sockaddr_in clientSockAdr;
    socklen_t clientSockSize = sizeof(clientSockAdr);

    /*fd_set fdRead;
    FD_ZERO(&fdRead);
    FD_SET(socket_,&fdRead);
    struct timeval timeOutTime;// = {timeout,0};

    timeOutTime.tv_sec = timeout;
    timeOutTime.tv_usec = 0;

    int retVal = select(1,&fdRead,NULL,NULL,&timeOutTime);
    if (retVal <= 0)
    {
        return std::unique_ptr<tcp_stream_socket>();
    }

    if (!FD_ISSET(socket_,&fdRead))
    {
        return std::unique_ptr<tcp_stream_socket>();
    }*/

    Socket clientSocket = accept(socket_, reinterpret_cast<sockaddr*>(&clientSockAdr),&clientSockSize);
    if (clientSocket == INVALID_SOCKET)
    {
        return std::unique_ptr<tcp_stream_socket>();
    }

    return std::make_unique<tcp_stream_socket>(clientSocket);    
} 

void tcp_server_socket::closeSocket()
{
    if (socket_ != INVALID_SOCKET)
    {
#if WIN32
        closesocket(socket_);
        WSACleanup();
#else
        close(socket_);
#endif
    }
}

} //namespace hallos