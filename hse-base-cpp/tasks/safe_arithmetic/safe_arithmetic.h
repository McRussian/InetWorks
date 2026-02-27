#pragma once

#include <cstdint>

bool SafeAdd(int64_t a, int64_t b, int64_t& result);

bool SafeSubtract(int64_t a, int64_t b, int64_t& result);

bool SafeMultiply(int64_t a, int64_t b, int64_t& result);

bool SafeDivide(int64_t a, int64_t b, int64_t& result);
