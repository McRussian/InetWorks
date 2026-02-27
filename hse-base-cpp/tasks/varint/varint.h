#pragma once

#include <cstddef>
#include <cstdint>

// Decode a varint from a byte sequence and return the number of bytes consumed.
// Sets 'result' to the decoded value, if the varint is valid.
// Returns 0 iff the varint is invalid or the buffer is too short.
size_t DecodeVarint(const uint8_t* data, size_t size, uint64_t& result);
