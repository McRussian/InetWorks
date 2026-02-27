#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

using Grid = std::vector<std::vector<uint8_t>>;

class GameOfLife {
public:
    explicit GameOfLife(const Grid& initial_state);

    void Simulate(uint64_t steps);

    size_t Rows() const;
    size_t Cols() const;
    bool IsAlive(size_t row, size_t col) const;
    Grid GetState() const;
};
