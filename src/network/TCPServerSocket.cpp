#include "TCPServerSocket.h"

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

#include "Logger.h"

/**
 * Constructor
 */ 
TCPServerSocket::TCPServerSocket(int port)
{
#if WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0)
    {
        throw TCPSocketException("Failed to startup winsock.");
    }
    if (!(LOBYTE(wsaData.wVersion) >= 2))
    {
        throw TCPSocketException("Too old winsock version.");
    }
#endif //WIN32

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET)
    {
        Logger::log("TCPServerSocket: Could not create socket.");
        throw TCPSocketException("Could not create socket.");
    } 

    linger so_linger = {1, 0};
    setsockopt(socket_, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

    sockaddr_in sockAdr = {0};
    sockAdr.sin_family = AF_INET;
    sockAdr.sin_port = htons(port);  
    sockAdr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_, reinterpret_cast<sockaddr*>(&sockAdr), sizeof(sockAdr)) != 0)
    {
        Logger::log("TCPServerSocket: Couldn't bind socket. Error code: " + std::to_string(errno));
        closeSocket();
        throw TCPSocketException("Could not bind socket.");
    }

    if (listen(socket_, SOMAXCONN)!=0)
    {
        Logger::log("TCPServerSocket: Couldn't set socket in listening mode. Error code: " + std::to_string(errno));
        closeSocket();
        throw TCPSocketException("Could not set socket in listening mode.");
    }   
}

/**
 * Destructor
 */ 
TCPServerSocket::~TCPServerSocket()
{
    closeSocket();
}

/**
 * 
 */
std::unique_ptr<ITCPStreamSocket> TCPServerSocket::acceptConnection()
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
        return std::unique_ptr<TCPStreamSocket>();
    }

    if (!FD_ISSET(socket_,&fdRead))
    {
        return std::unique_ptr<TCPStreamSocket>();
    }*/

    Socket clientSocket = accept(socket_, reinterpret_cast<sockaddr*>(&clientSockAdr),&clientSockSize);
    if (clientSocket == INVALID_SOCKET)
    {
        return std::unique_ptr<TCPStreamSocket>();
    }

    return std::make_unique<TCPStreamSocket>(clientSocket);    
} 

void TCPServerSocket::closeSocket()
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