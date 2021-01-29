#ifndef TCPSTREAMSOCKET_H
#define TCPSTREAMSOCKET_H

#ifndef WIN32
#define INVALID_SOCKET -1
#endif

#include <string>

typedef int Socket;

class ITCPStreamSocket
{
public:
    virtual ~ITCPStreamSocket() {};

    virtual std::string receiveData() = 0;
    virtual bool sendData(const std::string& buffer) = 0; 
};

class TCPStreamSocket : public ITCPStreamSocket
{
public:
    TCPStreamSocket(Socket socket);
    virtual ~TCPStreamSocket();

    std::string receiveData();
    bool sendData(const std::string& buffer);
private:
    Socket socket_;
};

#endif