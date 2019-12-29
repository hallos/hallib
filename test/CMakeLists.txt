
add_executable(sort_test
    sort.cpp
)
add_test(
    NAME sort
    COMMAND sort_test
)

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