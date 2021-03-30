#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "threadpool.h"

#include <thread>
#include <vector>


TEST_CASE("Thread pool executes function and returns expected value")
{
    hallos::thread_pool threadPool(1);

    int value = 6;

    SECTION("Without arguments")
    {
        std::function<int()> func = [&value](){ return value; };
        auto future = threadPool.add_work(0, func);
        future.wait();
        REQUIRE(future.get() == value);
    }

    SECTION("With arguments")
    {
        std::function<int(int, int)> func = [](int val1, int val2){ return val1 + val2; };
        auto future = threadPool.add_work(0, func, value, value);
        future.wait();
        REQUIRE(future.get() == value + value);
    }
}

TEST_CASE("Thread pool executes functions in given priority order")
{
    hallos::thread_pool threadPool(1);

    std::vector<int> expected_outcome {0, 1, 2, 3, 4, 5, 6, 7, 8 ,9 };
    std::vector<int> result;

    std::function<void()> dummy_func = [](){ std::this_thread::sleep_for(std::chrono::milliseconds(400)); };
    std::function<void(int)> func = [&result](int index)
                                    {
                                        result.push_back(index);
                                    };

    threadPool.add_work(0, dummy_func);
    for (int i = 0; i < 10; i++)
    {
        threadPool.add_work(1+i, func, 9-i);
    }

    std::function<void()> dummy_func2 =[](){};
    auto future = threadPool.add_work(0, dummy_func2);
    future.wait();

    REQUIRE(result == expected_outcome);
}