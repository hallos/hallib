#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <vector>

#include "sort.h"

TEST_CASE("")
{
    std::vector<int> unsorted{10, 1, 3, 9, 6, 2, 5, 4, 8, 7};
    std::vector<int> expected_outcome = unsorted;
    std::sort(expected_outcome.begin(), expected_outcome.end());

    hallos::mergesort(unsorted.begin(), unsorted.end());

    REQUIRE(unsorted == expected_outcome);
}