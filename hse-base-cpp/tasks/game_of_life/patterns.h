#pragma once

#include "game_of_life.h"

#include <cstdlib>
#include <string>
#include <vector>

namespace patterns {

// Centers a text pattern on a field of given size.
// '#' = alive, anything else = dead.
inline Grid MakeGrid(const std::vector<std::string>& pattern, size_t rows, size_t cols) {
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

inline Grid Block(size_t rows = 16, size_t cols = 16) {
    return MakeGrid({
        "##",
        "##",
    }, rows, cols);
}

inline Grid Beehive(size_t rows = 16, size_t cols = 16) {
    return MakeGrid({
        ".##.",
        "#..#",
        ".##.",
    }, rows, cols);
}

inline Grid Loaf(size_t rows = 16, size_t cols = 16) {
    return MakeGrid({
        ".##.",
        "#..#",
        ".#.#",
        "..#.",
    }, rows, cols);
}

inline Grid Blinker(size_t rows = 16, size_t cols = 16) {
    return MakeGrid({"###"}, rows, cols);
}

inline Grid Toad(size_t rows = 16, size_t cols = 16) {
    return MakeGrid({
        ".###",
        "###.",
    }, rows, cols);
}

inline Grid Pulsar(size_t rows = 32, size_t cols = 32) {
    return MakeGrid({
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
    }, rows, cols);
}

inline Grid Pentadecathlon(size_t rows = 32, size_t cols = 32) {
    return MakeGrid({
        "###",
        "#.#",
        "###",
        "###",
        "###",
        "###",
        "#.#",
        "###",
    }, rows, cols);
}

inline Grid Glider(size_t rows = 32, size_t cols = 32) {
    return MakeGrid({
        ".#.",
        "..#",
        "###",
    }, rows, cols);
}

inline Grid LWSS(size_t rows = 32, size_t cols = 32) {
    return MakeGrid({
        ".#..#",
        "#....",
        "#...#",
        "####.",
    }, rows, cols);
}

inline Grid GosperGliderGun(size_t rows = 48, size_t cols = 64) {
    return MakeGrid({
        "........................#...........",
        "......................#.#...........",
        "............##......##............##",
        "...........#...#....##............##",
        "##........#.....#...##..............",
        "##........#...#.##....#.#...........",
        "..........#.....#.......#...........",
        "...........#...#....................",
        "............##......................",
    }, rows, cols);
}

inline Grid RPentomino(size_t rows = 64, size_t cols = 64) {
    return MakeGrid({
        ".##",
        "##.",
        ".#.",
    }, rows, cols);
}

// --- Random ---

inline Grid Random(size_t rows = 32, size_t cols = 32, double density = 0.3,
                    unsigned seed = 42) {
    Grid grid(rows, std::vector<uint8_t>(cols, 0));
    std::srand(seed);
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            grid[r][c] = (static_cast<double>(std::rand()) / RAND_MAX) < density ? 1 : 0;
        }
    }
    return grid;
}

}  // namespace patterns
