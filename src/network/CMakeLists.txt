
#include_directories(../)
add_library(hallos_server STATIC
    tcp_server.cpp
    tcp_server_socket.cpp
    tcp_stream_socket.cpp
)

target_link_libraries(hallos_server
    pthread
)