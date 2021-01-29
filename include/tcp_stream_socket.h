#ifndef TCPSTREAMSOCKET_H
#define TCPSTREAMSOCKET_H

#ifndef WIN32
#define INVALID_SOCKET -1
#endif

#include <string>

typedef int Socket;

namespace hallos
{

class Itcp_stream_socket
{
public:
    virtual ~Itcp_stream_socket() {};

    virtual std::string receiveData() = 0;
    virtual bool sendData(const std::string& buffer) = 0; 
};

class tcp_stream_socket : public Itcp_stream_socket
{
public:
    tcp_stream_socket(Socket socket);
    virtual ~tcp_stream_socket();

    std::string receiveData();
    bool sendData(const std::string& buffer);
private:
    Socket socket_;
};

} //namespace hallos

#endif