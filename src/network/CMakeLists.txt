
include_directories(../)
add_library(hallos_server STATIC
    tcp_server.cpp
    TCPServerSocket.cpp
    TCPStreamSocket.cpp
)

target_link_libraries(hallos_server
    pthread
)