#include "game_of_life.h"

GameOfLife::GameOfLife(const Grid& /*initial_state*/) {
}

void GameOfLife::Simulate(uint64_t /*steps*/) {
}

size_t GameOfLife::Rows() const {
    return 0;
}

size_t GameOfLife::Cols() const {
    return 0;
}

bool GameOfLife::IsAlive(size_t /*row*/, size_t /*col*/) const {
    return false;
}

Grid GameOfLife::GetState() const {
    return {};
}
