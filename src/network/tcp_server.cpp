#include "tcp_server.h"
#include <vector>
#include "TCPStreamSocket.h"
//#include "Logger.h"


tcp_server::tcp_server(std::shared_ptr<ITCPServerSocket> serverSocket,
               std::shared_ptr<connection_handler> connectionHandler,
               int numThreads) :
                    run_(false),
                    serverSocket_(serverSocket),
                    connection_handler_(connectionHandler)
{
    threadPool_ = std::make_shared<hallos::thread_pool>(numThreads);
}

tcp_server::~tcp_server()
{
    run_ = false;
    serverThread_->join();
}

bool tcp_server::startServer()
{
    if (!run_)
    {
        std::lock_guard<std::mutex> lock(runMutex_);
        run_ = true;
        serverThread_ = std::make_unique<std::thread>(&tcp_server::runServer, this);
        return true;
    }
    else
    {
//        Logger::log("tcp_server::startServer(): tcp_server is already running.");
        return false;
    }
}

void tcp_server::stopServer()
{
    runMutex_.lock();
    run_ = false;
    runMutex_.unlock();
}

bool tcp_server::isRunning()
{
    return run_;
}

void tcp_server::runServer(){
    try
    {

        std::function<void(std::shared_ptr<ITCPStreamSocket>)> handler = [connHandler = connection_handler_](std::shared_ptr<ITCPStreamSocket> clientSocket)
        {
            connHandler->onAccept(clientSocket);
        };

        while(this->isRunning())
        {
            auto clientSocket = serverSocket_->acceptConnection();
            if (clientSocket)
            {    
                std::shared_ptr<ITCPStreamSocket> sharedSocket = std::move(clientSocket);
                threadPool_->add_work(handler, sharedSocket);
            }          
        }
    }
    catch (TCPSocketException& e)
    {
//        Logger::log("tcp_server::runServer(): Socket failed: " + e.what());
//        Logger::log("tcp_server::runServer(): Shutting down server");
        stopServer();
        return;
    }
}