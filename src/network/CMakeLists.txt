
include_directories(../)
add_library(hallos_network STATIC
    tcp_server.cpp
    tcp_server_socket.cpp
    tcp_stream_socket.cpp
)

target_link_libraries(hallos_network
    pthread
    hallos_osal
)