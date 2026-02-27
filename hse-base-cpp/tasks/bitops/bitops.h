#pragma once

#include <cstddef>
#include <cstdint>

// Swap bytes in a 64-bit value. Often used to swap endianness.
uint64_t SwapBytes(uint64_t value);

// Reverse bits in a 64-bit value.
uint64_t ReverseBits(uint64_t value);

// Reverse bits in each byte of a 64-bit value.
uint64_t ReverseBitsInBytes(uint64_t value);

// Set count bits starting from offset to the value of bits.
uint64_t SetBits(uint64_t value, uint64_t offset, uint64_t count, uint64_t bits);

// Extract count bits starting from offset.
uint64_t ExtractBits(uint64_t value, uint64_t offset, uint64_t count);

// Count number of set bits (1s) in value.
uint32_t CountSetBits(uint64_t value);

// Count number of trailing zeros in value.
uint32_t CountTrailingZeros(uint64_t value);

// Count number of leading zeros in value.
uint32_t CountLeadingZeros(uint64_t value);

// Rotate bits left by shift positions. Shift may be greater than 64.
uint64_t RotateLeft(uint64_t value, uint32_t shift);

// Rotate bits right by shift positions. Shift may be greater than 64.
uint64_t RotateRight(uint64_t value, uint32_t shift);

// Check if value is a power of two.
bool IsPowerOfTwo(uint64_t value);

// Find the smallest power of two that is greater than or equal to value.
uint64_t RoundUpToPowerOfTwo(uint64_t value);

// Find the largest number that is less than or equal to value and is divisible by alignment.
// Alignment must be a power of two.
uint64_t AlignDown(uint64_t value, uint64_t alignment);

// Find the smallest number that is greater than or equal to value and is divisible by alignment.
// Alignment must be a power of two.
uint64_t AlignUp(uint64_t value, uint64_t alignment);
