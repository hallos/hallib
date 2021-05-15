#include "osal.h"

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
namespace os
{

int socket_create(socket_type type)
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

    int sock_type;
    switch (type)
    {
    case UDP:
        sock_type = SOCK_DGRAM;
        break;
    case TCP:
    default:
        sock_type = SOCK_STREAM;
        break; 
    }

    int sock = socket(AF_INET, sock_type, IPPROTO_TCP);
    if (sock < 0)
    {
        // log error
    } 
    return sock;
}

bool socket_bind(int socket, int port)
{
    sockaddr_in sockAdr = {0};
    sockAdr.sin_family = AF_INET;
    sockAdr.sin_port = htons(port);  
    sockAdr.sin_addr.s_addr = htonl(INADDR_ANY);

    return (bind(socket, reinterpret_cast<sockaddr*>(&sockAdr), sizeof(sockAdr)) == 0);
}

bool socket_listen(int socket)
{
    return (listen(socket, SOMAXCONN) == 0);
}

void socket_close(int socket)
{
    if (socket >= 0)
    {
#if WIN32
        closesocket(socket_);
        WSACleanup();
#else
        close(socket);
#endif
    }
}

int socket_accept(int socket)
{
    sockaddr_in clientSockAdr;
    socklen_t clientSockSize = sizeof(clientSockAdr);

    int client_socket = accept(socket, reinterpret_cast<sockaddr*>(&clientSockAdr),&clientSockSize);

    return client_socket;
}

int socket_receive(int socket, char* buffer, int buffer_size)
{
    return recv(socket, buffer, buffer_size, 0);  
}

int socket_send(int socket, const char* buffer, int buffer_size)
{
    return send(socket, buffer, buffer_size, 0);
}

int socket_set_so_linger(int socket, int arg1, int arg2)
{
    linger so_linger = {arg1, arg2};
    setsockopt(socket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));  
}

};
};