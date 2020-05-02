
include_directories(../)
add_library(Server STATIC
    Server.cpp
    TCPServerSocket.cpp
    TCPStreamSocket.cpp
)

target_link_libraries(Server
    pthread
)