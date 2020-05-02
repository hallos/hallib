
include_directories(../)
add_library(Server STATIC
    tcp_server.cpp
    TCPServerSocket.cpp
    TCPStreamSocket.cpp
)

target_link_libraries(Server
    pthread
)