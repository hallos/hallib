#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <functional>
#include "TCPServerSocket.h"
#include "threadpool.h"


class ConnectionHandler
{
public:
    ConnectionHandler() {};
    virtual ~ConnectionHandler() {};
    virtual void onAccept(std::shared_ptr<ITCPStreamSocket> clientSocket) = 0;
};

class Server 
{
public:
    Server(std::shared_ptr<ITCPServerSocket> serverSocket,
           std::shared_ptr<ConnectionHandler> connectionHandler,
           int numThreads);
    ~Server();

    bool isRunning();
    bool startServer();
    void stopServer();
private:
    bool run_;
    std::mutex runMutex_;
    std::unique_ptr<std::thread> serverThread_;
    std::shared_ptr<hallos::thread_pool> threadPool_;
    std::shared_ptr<ITCPServerSocket> serverSocket_;
    std::shared_ptr<ConnectionHandler> connectionHandler_;

    void runServer();
};

#endif
