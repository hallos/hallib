#include "TCPStreamSocket.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> //close
#endif

#define MAX_SEGMENT_SIZE 2000



/**
 * Constructor
 */ 
TCPStreamSocket::TCPStreamSocket(Socket socket): socket_(socket) {}

/**
 * Destructor
 */ 
TCPStreamSocket::~TCPStreamSocket()
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

/**
 * 
 */
std::string TCPStreamSocket::receiveData()
{
    char recBuffer[MAX_SEGMENT_SIZE];
    int numBytes = recv(socket_, recBuffer, sizeof(recBuffer), 0);
    if (numBytes == -1 || numBytes == 0)
    {
        return std::string();
    }
    return std::string(recBuffer, numBytes);
} 

/**
 * 
 */ 
bool TCPStreamSocket::sendData(const std::string& buffer)
{
    if (send(socket_, buffer.data(), buffer.size(), 0) == -1)
    {   
        //TODO: Log error
        return false;
    }
    return true;
}