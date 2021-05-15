#include "tcp_stream_socket.h"

#include "osal.h"

#define MAX_SEGMENT_SIZE 2000

namespace hallos
{

/**
 * Constructor
 */ 
tcp_stream_socket::tcp_stream_socket(Socket socket): socket_(socket) {}

/**
 * Destructor
 */ 
tcp_stream_socket::~tcp_stream_socket()
{
    if (socket_ != INVALID_SOCKET)
    {
        os::socket_close(socket_);
    }
}

/**
 * 
 */
std::string tcp_stream_socket::receiveData()
{
    char recBuffer[MAX_SEGMENT_SIZE];
    int numBytes = os::socket_receive(socket_, recBuffer, sizeof(recBuffer));
    if (numBytes == -1 || numBytes == 0)
    {
        return std::string();
    }
    return std::string(recBuffer, numBytes);
} 

/**
 * 
 */ 
bool tcp_stream_socket::sendData(const std::string& buffer)
{
    if (os::socket_send(socket_, buffer.data(), buffer.size()) == -1)
    {   
        //TODO: Log error
        return false;
    }
    return true;
}

} //namespace hallos