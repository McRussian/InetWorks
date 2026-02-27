#include <catch.hpp>

#include "game_of_life.h"

#include <string>
#include <vector>

#include "time_limiter.h"

namespace {

Grid MakeGrid(const std::vector<std::string>& pattern, size_t rows, size_t cols) {
    Grid grid(rows, std::vector<uint8_t>(cols, 0));
    size_t off_r = (rows - pattern.size()) / 2;
    for (size_t r = 0; r < pattern.size(); ++r) {
        size_t off_c = (cols - pattern[r].size()) / 2;
        for (size_t c = 0; c < pattern[r].size(); ++c) {
            grid[off_r + r][off_c + c] = (pattern[r][c] == '#') ? 1 : 0;
        }
    }
    return grid;
}

size_t CountAlive(const GameOfLife& game) {
    size_t count = 0;
    for (size_t r = 0; r < game.Rows(); ++r) {
        for (size_t c = 0; c < game.Cols(); ++c) {
            if (game.IsAlive(r, c)) {
                ++count;
            }
        }
    }
    return count;
}

}  // namespace

// ============================================================
// Basic tests — Simulate(1) as a single step
// ============================================================

TEST_CASE("Empty field") {
    Grid empty(8, std::vector<uint8_t>(8, 0));
    GameOfLife game(empty);
    game.Simulate(1);
    REQUIRE(game.GetState() == empty);
}

TEST_CASE("Single cell dies") {
    auto grid = MakeGrid({"#"}, 8, 8);
    GameOfLife game(grid);
    game.Simulate(1);

    Grid empty(8, std::vector<uint8_t>(8, 0));
    REQUIRE(game.GetState() == empty);
}

TEST_CASE("Block is stable") {
    auto grid = MakeGrid({
        "##",
        "##",
    }, 8, 8);

    GameOfLife game(grid);
    auto initial = game.GetState();
    game.Simulate(1);
    REQUIRE(game.GetState() == initial);
    game.Simulate(10);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Beehive is stable") {
    auto grid = MakeGrid({
        ".##.",
        "#..#",
        ".##.",
    }, 8, 8);

    GameOfLife game(grid);
    auto initial = game.GetState();
    game.Simulate(1);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Loaf is stable") {
    auto grid = MakeGrid({
        ".##.",
        "#..#",
        ".#.#",
        "..#.",
    }, 10, 10);

    GameOfLife game(grid);
    auto initial = game.GetState();
    game.Simulate(1);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Blinker period 2") {
    auto grid = MakeGrid({"###"}, 8, 8);

    GameOfLife game(grid);
    auto initial = game.GetState();

    game.Simulate(1);
    REQUIRE(game.GetState() != initial);
    REQUIRE(CountAlive(game) == 3);

    game.Simulate(1);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Toad period 2") {
    auto grid = MakeGrid({
        ".###",
        "###.",
    }, 8, 8);

    GameOfLife game(grid);
    auto initial = game.GetState();

    game.Simulate(1);
    REQUIRE(game.GetState() != initial);

    game.Simulate(1);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Pulsar period 3") {
    auto grid = MakeGrid({
        "..###...###..",
        ".............",
        "#....#.#....#",
        "#....#.#....#",
        "#....#.#....#",
        "..###...###..",
        ".............",
        "..###...###..",
        "#....#.#....#",
        "#....#.#....#",
        "#....#.#....#",
        ".............",
        "..###...###..",
    }, 20, 20);

    GameOfLife game(grid);
    auto initial = game.GetState();

    game.Simulate(1);
    REQUIRE(game.GetState() != initial);

    game.Simulate(1);
    REQUIRE(game.GetState() != initial);

    game.Simulate(1);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Glider moves") {
    auto grid = MakeGrid({
        ".#.",
        "..#",
        "###",
    }, 16, 16);

    GameOfLife game(grid);
    auto initial = game.GetState();

    game.Simulate(4);
    // After 4 steps, glider shifts by (1, 1)
    REQUIRE(game.GetState() != initial);
    REQUIRE(CountAlive(game) == 5);
}

TEST_CASE("Torus wrapping") {
    // Place a glider near the top-left corner, it should wrap around
    Grid grid(8, std::vector<uint8_t>(8, 0));
    // Glider at row 0, col 0 area
    grid[0][1] = 1;
    grid[1][2] = 1;
    grid[2][0] = 1;
    grid[2][1] = 1;
    grid[2][2] = 1;

    GameOfLife game(grid);
    // Simulate enough steps for the glider to wrap around on an 8x8 torus
    // Glider moves by (1,1) every 4 ticks. After 4*8=32 ticks it should return.
    auto initial = game.GetState();
    game.Simulate(32);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Dimensions are correct") {
    Grid grid(5, std::vector<uint8_t>(7, 0));
    GameOfLife game(grid);
    REQUIRE(game.Rows() == 5);
    REQUIRE(game.Cols() == 7);
}

TEST_CASE("GetState returns current state") {
    auto grid = MakeGrid({"##", "##"}, 8, 8);
    GameOfLife game(grid);
    REQUIRE(game.GetState() == grid);
}

TEST_CASE("Simulate zero steps") {
    auto grid = MakeGrid({"###"}, 8, 8);
    GameOfLife game(grid);
    auto initial = game.GetState();
    game.Simulate(0);
    REQUIRE(game.GetState() == initial);
}

// ============================================================
// Advanced tests — Simulate with huge step counts
// ============================================================

TEST_CASE("Blinker huge even steps") {
    TimeLimiter limiter(std::chrono::milliseconds(5000));

    auto grid = MakeGrid({"###"}, 8, 8);
    GameOfLife game(grid);
    auto initial = game.GetState();

    game.Simulate(1'000'000'000'000'000'000ULL);  // 10^18, even
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Blinker huge odd steps") {
    TimeLimiter limiter(std::chrono::milliseconds(5000));

    auto grid = MakeGrid({"###"}, 8, 8);
    GameOfLife game(grid);
    auto initial = game.GetState();

    // Get the state after 1 step for comparison
    GameOfLife game2(grid);
    game2.Simulate(1);
    auto after_one = game2.GetState();

    game.Simulate(1'000'000'000'000'000'001ULL);  // 10^18 + 1, odd
    REQUIRE(game.GetState() == after_one);
}

TEST_CASE("Block huge steps") {
    TimeLimiter limiter(std::chrono::milliseconds(5000));

    auto grid = MakeGrid({
        "##",
        "##",
    }, 8, 8);
    GameOfLife game(grid);
    auto initial = game.GetState();

    game.Simulate(1'000'000'000'000'000'000ULL);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Pulsar huge steps") {
    TimeLimiter limiter(std::chrono::milliseconds(5000));

    auto grid = MakeGrid({
        "..###...###..",
        ".............",
        "#....#.#....#",
        "#....#.#....#",
        "#....#.#....#",
        "..###...###..",
        ".............",
        "..###...###..",
        "#....#.#....#",
        "#....#.#....#",
        "#....#.#....#",
        ".............",
        "..###...###..",
    }, 20, 20);

    GameOfLife game(grid);
    auto initial = game.GetState();

    // 3 * 10^17 is divisible by period 3
    game.Simulate(300'000'000'000'000'000ULL);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Glider huge steps on torus") {
    TimeLimiter limiter(std::chrono::milliseconds(5000));

    // Glider on 8x8 torus: returns every 32 steps (4 ticks per move * 8 rows)
    Grid grid(8, std::vector<uint8_t>(8, 0));
    grid[0][1] = 1;
    grid[1][2] = 1;
    grid[2][0] = 1;
    grid[2][1] = 1;
    grid[2][2] = 1;

    GameOfLife game(grid);
    auto initial = game.GetState();

    // 32 * 10^15 is divisible by 32
    game.Simulate(32'000'000'000'000'000ULL);
    REQUIRE(game.GetState() == initial);
}

TEST_CASE("Pentadecathlon huge steps") {
    TimeLimiter limiter(std::chrono::milliseconds(5000));

    auto grid = MakeGrid({
        "###",
        "#.#",
        "###",
        "###",
        "###",
        "###",
        "#.#",
        "###",
    }, 20, 20);

    GameOfLife game(grid);
    auto initial = game.GetState();

    // 15 * 10^16 is divisible by period 15
    game.Simulate(150'000'000'000'000'000ULL);
    REQUIRE(game.GetState() == initial);
}
