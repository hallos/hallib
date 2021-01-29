# Sort test
add_executable(sort_test
    sort.cpp
)
add_test(
    NAME sort
    COMMAND sort_test
)

# Threadpool test
add_executable(threadpool_test
    threadpool_test.cpp
)
target_link_libraries(threadpool_test
    pthread
)
add_test(
    NAME threadpool
    COMMAND threadpool_test
)

# Network test
add_executable(network_test
    network_test.cpp
    server.cpp
)
target_link_libraries(network_test
    pthread
    hallos_network
)
add_test(
    NAME network
    COMMAND network_test
)