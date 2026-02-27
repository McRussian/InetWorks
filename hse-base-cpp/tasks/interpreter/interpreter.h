#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

class Interpreter {
public:
    explicit Interpreter(size_t tape_size);

    std::string Run(std::string_view code, std::string_view input);

private:
    std::vector<uint8_t> tape_;
};
