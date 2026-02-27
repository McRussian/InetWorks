#include "safe_arithmetic.h"

#include <catch.hpp>
#include <limits>

TEST_CASE("SafeAdd basic operations") {
    int64_t result = 42;

    SECTION("Simple positive addition") {
        CHECK(SafeAdd(10, 20, result) == true);
        CHECK(result == 30);
    }

    SECTION("Addition with zero") {
        CHECK(SafeAdd(100, 0, result) == true);
        CHECK(result == 100);

        CHECK(SafeAdd(0, -50, result) == true);
        CHECK(result == -50);
    }

    SECTION("Negative numbers") {
        CHECK(SafeAdd(-10, -20, result) == true);
        CHECK(result == -30);

        CHECK(SafeAdd(-100, 150, result) == true);
        CHECK(result == 50);
    }
}

TEST_CASE("SafeAdd overflow detection") {
    int64_t result = 42;
    const int64_t max_val = std::numeric_limits<int64_t>::max();
    const int64_t min_val = std::numeric_limits<int64_t>::min();

    SECTION("Positive overflow") {
        CHECK(SafeAdd(max_val, 1, result) == false);
        CHECK(result == 42);

        CHECK(SafeAdd(max_val, max_val, result) == false);
        CHECK(result == 42);
    }

    SECTION("Negative overflow") {
        CHECK(SafeAdd(min_val, -1, result) == false);
        CHECK(result == 42);

        CHECK(SafeAdd(min_val, min_val, result) == false);
        CHECK(result == 42);
    }

    SECTION("Boundary cases that should work") {
        CHECK(SafeAdd(max_val, 0, result) == true);
        CHECK(result == max_val);

        CHECK(SafeAdd(min_val, 0, result) == true);
        CHECK(result == min_val);

        CHECK(SafeAdd(max_val, min_val, result) == true);
        CHECK(result == -1);
    }
}

TEST_CASE("SafeSubtract basic operations") {
    int64_t result = 42;

    SECTION("Simple subtraction") {
        CHECK(SafeSubtract(20, 10, result) == true);
        CHECK(result == 10);

        CHECK(SafeSubtract(10, 20, result) == true);
        CHECK(result == -10);
    }

    SECTION("Subtraction with zero") {
        CHECK(SafeSubtract(100, 0, result) == true);
        CHECK(result == 100);

        CHECK(SafeSubtract(0, 50, result) == true);
        CHECK(result == -50);
    }

    SECTION("Negative numbers") {
        CHECK(SafeSubtract(-10, -20, result) == true);
        CHECK(result == 10);

        CHECK(SafeSubtract(-20, 10, result) == true);
        CHECK(result == -30);
    }
}

TEST_CASE("SafeSubtract overflow detection") {
    int64_t result = 42;
    const int64_t max_val = std::numeric_limits<int64_t>::max();
    const int64_t min_val = std::numeric_limits<int64_t>::min();

    SECTION("Positive overflow") {
        CHECK(SafeSubtract(max_val, -1, result) == false);
        CHECK(result == 42);

        CHECK(SafeSubtract(max_val, min_val, result) == false);
        CHECK(result == 42);
    }

    SECTION("Negative overflow") {
        CHECK(SafeSubtract(min_val, 1, result) == false);
        CHECK(result == 42);

        CHECK(SafeSubtract(min_val, max_val, result) == false);
        CHECK(result == 42);
    }

    SECTION("Boundary cases that should work") {
        CHECK(SafeSubtract(max_val, 0, result) == true);
        CHECK(result == max_val);

        CHECK(SafeSubtract(min_val, 0, result) == true);
        CHECK(result == min_val);

        CHECK(SafeSubtract(max_val, max_val, result) == true);
        CHECK(result == 0);
    }
}

TEST_CASE("SafeMultiply basic operations") {
    int64_t result = 42;

    SECTION("Simple multiplication") {
        CHECK(SafeMultiply(10, 20, result) == true);
        CHECK(result == 200);

        CHECK(SafeMultiply(-10, 20, result) == true);
        CHECK(result == -200);

        CHECK(SafeMultiply(-10, -20, result) == true);
        CHECK(result == 200);
    }

    SECTION("Multiplication with zero") {
        CHECK(SafeMultiply(0, 1000000000000LL, result) == true);
        CHECK(result == 0);

        CHECK(SafeMultiply(1000000000000LL, 0, result) == true);
        CHECK(result == 0);
    }

    SECTION("Multiplication with one") {
        CHECK(SafeMultiply(123456789, 1, result) == true);
        CHECK(result == 123456789);

        CHECK(SafeMultiply(-123456789, 1, result) == true);
        CHECK(result == -123456789);
    }
}

TEST_CASE("SafeMultiply overflow detection") {
    int64_t result = 42;
    const int64_t max_val = std::numeric_limits<int64_t>::max();
    const int64_t min_val = std::numeric_limits<int64_t>::min();

    SECTION("Large positive numbers") {
        CHECK(SafeMultiply(max_val, 2, result) == false);
        CHECK(result == 42);

        CHECK(SafeMultiply(1000000000000LL, 1000000000000LL, result) == false);
        CHECK(result == 42);
    }

    SECTION("Large negative numbers") {
        CHECK(SafeMultiply(min_val, 2, result) == false);
        CHECK(result == 42);

        CHECK(SafeMultiply(-1000000000000LL, 1000000000000LL, result) == false);
        CHECK(result == 42);
    }

    SECTION("Edge case: min_val * -1") {
        CHECK(SafeMultiply(min_val, -1, result) == false);
        CHECK(result == 42);
    }

    SECTION("Boundary cases that should work") {
        CHECK(SafeMultiply(max_val, 1, result) == true);
        CHECK(result == max_val);

        CHECK(SafeMultiply(min_val, 1, result) == true);
        CHECK(result == min_val);

        // Square root of max_val should be safe to multiply
        int64_t sqrt_max = 3037000499LL;  // approximately sqrt(INT64_MAX)
        CHECK(SafeMultiply(sqrt_max, sqrt_max, result) == true);
    }
}

TEST_CASE("SafeDivide basic operations") {
    int64_t result = 42;

    SECTION("Simple division") {
        CHECK(SafeDivide(20, 10, result) == true);
        CHECK(result == 2);

        CHECK(SafeDivide(-20, 10, result) == true);
        CHECK(result == -2);

        CHECK(SafeDivide(-20, -10, result) == true);
        CHECK(result == 2);
    }

    SECTION("Division with remainder") {
        CHECK(SafeDivide(23, 10, result) == true);
        CHECK(result == 2);

        CHECK(SafeDivide(-23, 10, result) == true);
        CHECK(result == -2);
    }

    SECTION("Division by one") {
        CHECK(SafeDivide(123456789, 1, result) == true);
        CHECK(result == 123456789);

        CHECK(SafeDivide(-123456789, 1, result) == true);
        CHECK(result == -123456789);
    }
}

TEST_CASE("SafeDivide error cases") {
    int64_t result = 42;
    const int64_t min_val = std::numeric_limits<int64_t>::min();

    SECTION("Division by zero") {
        CHECK(SafeDivide(100, 0, result) == false);
        CHECK(result == 42);

        CHECK(SafeDivide(-100, 0, result) == false);
        CHECK(result == 42);

        CHECK(SafeDivide(0, 0, result) == false);
        CHECK(result == 42);
    }

    SECTION("The only overflow case: INT64_MIN / -1") {
        CHECK(SafeDivide(min_val, -1, result) == false);
        CHECK(result == 42);
    }

    SECTION("Safe cases near boundaries") {
        CHECK(SafeDivide(min_val, 1, result) == true);
        CHECK(result == min_val);

        CHECK(SafeDivide(min_val, 2, result) == true);
        CHECK(result == min_val / 2);

        CHECK(SafeDivide(min_val, -2, result) == true);
        CHECK(result == min_val / -2);
    }
}

TEST_CASE("All operations with large numbers") {
    int64_t result = 42;

    // Use some large but safe numbers
    const int64_t large_pos = 1000000000000000LL;  // 10^15
    const int64_t large_neg = -1000000000000000LL;

    SECTION("Large safe additions") {
        CHECK(SafeAdd(large_pos, large_pos, result) == true);
        CHECK(result == 2 * large_pos);

        CHECK(SafeAdd(large_neg, large_neg, result) == true);
        CHECK(result == 2 * large_neg);
    }

    SECTION("Large safe subtractions") {
        CHECK(SafeSubtract(large_pos, large_neg, result) == true);
        CHECK(result == large_pos - large_neg);
    }

    SECTION("Large multiplications should overflow") {
        CHECK(SafeMultiply(large_pos, large_pos, result) == false);
        CHECK(result == 42);  // Should not be modified
    }

    SECTION("Large divisions should work") {
        CHECK(SafeDivide(large_pos, 1000, result) == true);
        CHECK(result == large_pos / 1000);
    }
}

// Quod licet Iovi, non licet bovi.
template <template <typename> typename Op>
bool ReallySafeOp(int64_t a, int64_t b, int64_t& result) {
    __int128_t wide_result = Op<__int128_t>{}(static_cast<__int128_t>(a), static_cast<__int128_t>(b));
    if (wide_result != static_cast<int64_t>(wide_result)) {
        return false;
    }
    result = static_cast<int64_t>(wide_result);
    return true;
}

TEST_CASE("Stress test") {
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int64_t> dist{
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int64_t>::max()
    };

    for (int i = 0; i < 10'000'000; ++i) {
        int64_t a = dist(rng);
        int64_t b = dist(rng);

        int64_t result = 42;
        int64_t expected = 42;

        REQUIRE(ReallySafeOp<std::plus>(a, b, expected) == SafeAdd(a, b, result));
        REQUIRE(expected == result);

        REQUIRE(ReallySafeOp<std::minus>(a, b, expected) == SafeSubtract(a, b, result));
        REQUIRE(expected == result);

        REQUIRE(ReallySafeOp<std::multiplies>(a, b, expected) == SafeMultiply(a, b, result));
        REQUIRE(expected == result);

        REQUIRE(ReallySafeOp<std::divides>(a, b, expected) == SafeDivide(a, b, result));
        REQUIRE(expected == result);
    }
}
