#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <functional>
#include "TCPServerSocket.h"
#include "threadpool.h"


class connection_handler
{
public:
    connection_handler() {};
    virtual ~connection_handler() {};
    virtual void onAccept(std::shared_ptr<ITCPStreamSocket> clientSocket) = 0;
};

class tcp_server 
{
public:
    tcp_server(std::shared_ptr<ITCPServerSocket> serverSocket,
           std::shared_ptr<connection_handler> connection_handler,
           int numThreads);
    ~tcp_server();

    bool isRunning();
    bool startServer();
    void stopServer();
private:
    bool run_;
    std::mutex runMutex_;
    std::unique_ptr<std::thread> serverThread_;
    std::shared_ptr<hallos::thread_pool> threadPool_;
    std::shared_ptr<ITCPServerSocket> serverSocket_;
    std::shared_ptr<connection_handler> connection_handler_;

    void runServer();
};

#endif
