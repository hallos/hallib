#include "Server.h"
#include <vector>
#include "TCPStreamSocket.h"
#include "Logger.h"


Server::Server(std::shared_ptr<ITCPServerSocket> serverSocket,
               std::shared_ptr<ConnectionHandler> connectionHandler,
               int numThreads) :
                    run_(false),
                    serverSocket_(serverSocket),
                    connectionHandler_(connectionHandler)
{
    threadPool_ = std::make_shared<hallos::thread_pool>(numThreads);
}

Server::~Server()
{
    run_ = false;
    serverThread_->join();
}

bool Server::startServer()
{
    if (!run_)
    {
        std::lock_guard<std::mutex> lock(runMutex_);
        run_ = true;
        serverThread_ = std::make_unique<std::thread>(&Server::runServer, this);
        return true;
    }
    else
    {
        Logger::log("Server::startServer(): Server is already running.");
        return false;
    }
}

void Server::stopServer()
{
    runMutex_.lock();
    run_ = false;
    runMutex_.unlock();
}

bool Server::isRunning()
{
    return run_;
}

void Server::runServer(){
    try
    {

        std::function<void(std::shared_ptr<ITCPStreamSocket>)> handler = [connHandler = connectionHandler_](std::shared_ptr<ITCPStreamSocket> clientSocket)
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
        Logger::log("Server::runServer(): Socket failed: " + e.what());
        Logger::log("Server::runServer(): Shutting down server");
        stopServer();
        return;
    }
}