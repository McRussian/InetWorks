#include <catch.hpp>

#include "defer.h"

TEST_CASE("Defer compiles") {
    defer {};
}

TEST_CASE("Multiple defers are supported") {
    defer {};
    defer {};
    defer {};
    defer {};
    defer {};
    defer {};
    defer {};
    defer {};
    defer {};
    defer {};
}

TEST_CASE("Defer accepts statements") {
    defer {
        int x = 0;
        REQUIRE(x + x == 0);
    };
}

TEST_CASE("Defer defers") {
    int x = 0;

    defer {
        ++x;
        REQUIRE(x == 2);
    };

    ++x;
}

TEST_CASE("Multiple defer statements are executed correctly") {
    int x = 0;

    defer {
        ++x;
        REQUIRE(x == 4);
    };

    defer {
        ++x;
        REQUIRE(x == 3);
    };

    defer {
        ++x;
        REQUIRE(x == 2);
    };

    ++x;
}

TEST_CASE("Defer in defer defers") {
    int x = 0;
    int defers = 0;

    {
        defer {
            defer {
                defer {
                    defer {
                        REQUIRE(x++ == 3);
                        defers++;
                    };
                    REQUIRE(x++ == 2);
                    defers++;
                };
                REQUIRE(x++ == 1);
                defers++;
            };
            REQUIRE(x++ == 0);
            defers++;
        };
    }

    REQUIRE(defers == 4);
}

TEST_CASE("Defer does not leave scope") {
    const int iterations = 100;
    int defers = 0;

    for (int i = 0; i < iterations; ++i) {
        defer {
            REQUIRE(i == defers);
            defers++;
        };

        REQUIRE(i == defers);
    }

    REQUIRE(defers == iterations);
}
