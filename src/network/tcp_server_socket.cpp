#include "tcp_server_socket.h"

#include "osal.h"


namespace hallos
{

/**
 * Constructor
 */ 
tcp_server_socket::tcp_server_socket(int port)
{
    socket_ = os::socket_create(os::TCP);
    if (socket_ == INVALID_SOCKET)
    {
        throw tcp_socket_exception("Could not create socket.");
    } 

   os::socket_set_so_linger(socket_, 1, 0);

    if(!os::socket_bind(socket_, port))
    {
        os::socket_close(socket_);
        throw tcp_socket_exception("Could not bind socket.");
    }

    if (!os::socket_listen(socket_))
    {
        os::socket_close(socket_);
        throw tcp_socket_exception("Could not set socket in listening mode.");
    }
}

/**
 * Destructor
 */ 
tcp_server_socket::~tcp_server_socket()
{
    os::socket_close(socket_);
}

/**
 * 
 */
std::unique_ptr<Itcp_stream_socket> tcp_server_socket::acceptConnection()
{
    Socket clientSocket = os::socket_accept(socket_);
    if (clientSocket == INVALID_SOCKET)
    {
        return std::unique_ptr<tcp_stream_socket>();
    }

    return std::make_unique<tcp_stream_socket>(clientSocket);    
} 

} //namespace hallos