#include "catch.hpp"

#include "minmax.h"

TEST_CASE("Public Simple") {
    {
        std::vector<int> a{1, 3, 2};
        auto [min, max] = MinMaxElement(a.begin(), a.end());
        REQUIRE(*min == 1);
        REQUIRE(*max == 3);
    }

    {
        std::vector<int> a{1, -3, 2};
        auto [min, max] = MinMaxElement(a.begin(), a.end());
        REQUIRE(*min == -3);
        REQUIRE(*max == 2);
        }

}

// NOLINTBEGIN
TEST_CASE("Public Other types") {
    {
        int a[5] = {1, 5, 3, 4, 2};
        auto [min, max] = MinMaxElement(a, a + 5);
        REQUIRE(*min == 1);
        REQUIRE(*max == 5);
    }
}
// NOLINTEND
