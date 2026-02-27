#include <catch.hpp>

#include "bitops.h"

class AreYouFastEnough {
public:
    AreYouFastEnough()
        : start_{std::clock()}
    {}

    ~AreYouFastEnough() {
        CHECK(std::clock() - start_ < CLOCKS_PER_SEC / 10000);
    }

private:
    std::clock_t start_;
};

TEST_CASE("SwapBytes") {
    AreYouFastEnough _;
    CHECK(SwapBytes(0x0123456789abcdef) == 0xefcdab8967452301);
    CHECK(SwapBytes(0x0000000000000000) == 0x0000000000000000);
    CHECK(SwapBytes(0xffffffffffffffff) == 0xffffffffffffffff);
    CHECK(SwapBytes(0x0000000000000001) == 0x0100000000000000);
    CHECK(SwapBytes(0x8000000000000000) == 0x0000000000000080);
}

TEST_CASE("ReverseBits") {
    AreYouFastEnough _;
    CHECK(ReverseBits(0x0000000000000001) == 0x8000000000000000);
    CHECK(ReverseBits(0x8000000000000000) == 0x0000000000000001);
    CHECK(ReverseBits(0x0000000000000000) == 0x0000000000000000);
    CHECK(ReverseBits(0xffffffffffffffff) == 0xffffffffffffffff);
    CHECK(ReverseBits(0x0123456789abcdef) == 0xf7b3d591e6a2c480);
}

TEST_CASE("SetBits") {
    AreYouFastEnough _;
    CHECK(SetBits(0x0000000000000000, 0, 4, 0xf) == 0x000000000000000f);
    CHECK(SetBits(0x0000000000000000, 4, 4, 0xf) == 0x00000000000000f0);
    CHECK(SetBits(0xffffffffffffffff, 8, 8, 0x00) == 0xffffffffffff00ff);
    CHECK(SetBits(0x0000000000000000, 60, 4, 0xf) == 0xf000000000000000);
    CHECK(SetBits(0x1234567890abcdef, 16, 16, 0x0000) == 0x123456780000cdef);
}

TEST_CASE("ExtractBits") {
    AreYouFastEnough _;
    CHECK(ExtractBits(0x000000000000000f, 0, 4) == 0xf);
    CHECK(ExtractBits(0x00000000000000f0, 4, 4) == 0xf);
    CHECK(ExtractBits(0xffffffffffff00ff, 8, 8) == 0x00);
    CHECK(ExtractBits(0xf000000000000000, 60, 4) == 0xf);
    CHECK(ExtractBits(0x1234567890abcdef, 16, 16) == 0x90ab);
}

TEST_CASE("CountSetBits") {
    AreYouFastEnough _;
    CHECK(CountSetBits(0x0000000000000000) == 0);
    CHECK(CountSetBits(0xffffffffffffffff) == 64);
    CHECK(CountSetBits(0x0000000000000001) == 1);
    CHECK(CountSetBits(0x8000000000000000) == 1);
    CHECK(CountSetBits(0x0123456789abcdef) == 32);
    CHECK(CountSetBits(0xaaaaaaaaaaaaaaaa) == 32);
    CHECK(CountSetBits(0x5555555555555555) == 32);
}

TEST_CASE("CountTrailingZeros") {
    AreYouFastEnough _;
    CHECK(CountTrailingZeros(0x0000000000000001) == 0);
    CHECK(CountTrailingZeros(0x0000000000000002) == 1);
    CHECK(CountTrailingZeros(0x0000000000000004) == 2);
    CHECK(CountTrailingZeros(0x0000000000000008) == 3);
    CHECK(CountTrailingZeros(0x8000000000000000) == 63);
    CHECK(CountTrailingZeros(0x0123456789abcde0) == 5);
    CHECK(CountTrailingZeros(0x0000000000000000) == 64);
}

TEST_CASE("CountLeadingZeros") {
    AreYouFastEnough _;
    CHECK(CountLeadingZeros(0x8000000000000000) == 0);
    CHECK(CountLeadingZeros(0x4000000000000000) == 1);
    CHECK(CountLeadingZeros(0x2000000000000000) == 2);
    CHECK(CountLeadingZeros(0x1000000000000000) == 3);
    CHECK(CountLeadingZeros(0x0000000000000001) == 63);
    CHECK(CountLeadingZeros(0x0123456789abcdef) == 7);
    CHECK(CountLeadingZeros(0x0000000000000000) == 64);
}

TEST_CASE("RotateLeft") {
    AreYouFastEnough _;
    CHECK(RotateLeft(0x0000000000000001, 1) == 0x0000000000000002);
    CHECK(RotateLeft(0x8000000000000000, 1) == 0x0000000000000001);
    CHECK(RotateLeft(0x0123456789abcdef, 8) == 0x23456789abcdef01);
    CHECK(RotateLeft(0x0123456789abcdef, 64) == 0x0123456789abcdef);
    CHECK(RotateLeft(0x0123456789abcdef, 0) == 0x0123456789abcdef);
    CHECK(RotateLeft(0xf000000000000000, 4) == 0x000000000000000f);
}

TEST_CASE("RotateRight") {
    AreYouFastEnough _;
    CHECK(RotateRight(0x0000000000000002, 1) == 0x0000000000000001);
    CHECK(RotateRight(0x0000000000000001, 1) == 0x8000000000000000);
    CHECK(RotateRight(0x0123456789abcdef, 8) == 0xef0123456789abcd);
    CHECK(RotateRight(0x0123456789abcdef, 64) == 0x0123456789abcdef);
    CHECK(RotateRight(0x0123456789abcdef, 0) == 0x0123456789abcdef);
    CHECK(RotateRight(0x000000000000000f, 4) == 0xf000000000000000);
}

TEST_CASE("IsPowerOfTwo") {
    AreYouFastEnough _;
    CHECK(IsPowerOfTwo(1));
    CHECK(IsPowerOfTwo(2));
    CHECK(IsPowerOfTwo(4));
    CHECK(IsPowerOfTwo(8));
    CHECK(IsPowerOfTwo(1024));
    CHECK(IsPowerOfTwo(0x8000000000000000));
    CHECK(!IsPowerOfTwo(0));
    CHECK(!IsPowerOfTwo(3));
    CHECK(!IsPowerOfTwo(5));
    CHECK(!IsPowerOfTwo(6));
    CHECK(!IsPowerOfTwo(1023));
    CHECK(!IsPowerOfTwo(0xffffffffffffffff));
}

TEST_CASE("RoundUpToPowerOfTwo") {
    AreYouFastEnough _;
    CHECK(RoundUpToPowerOfTwo(0) == 1);
    CHECK(RoundUpToPowerOfTwo(1) == 1);
    CHECK(RoundUpToPowerOfTwo(2) == 2);
    CHECK(RoundUpToPowerOfTwo(3) == 4);
    CHECK(RoundUpToPowerOfTwo(5) == 8);
    CHECK(RoundUpToPowerOfTwo(7) == 8);
    CHECK(RoundUpToPowerOfTwo(8) == 8);
    CHECK(RoundUpToPowerOfTwo(9) == 16);
    CHECK(RoundUpToPowerOfTwo(1023) == 1024);
    CHECK(RoundUpToPowerOfTwo(1024) == 1024);
    CHECK(RoundUpToPowerOfTwo(1025) == 2048);
    CHECK(RoundUpToPowerOfTwo(64444) == 65536);
    CHECK(RoundUpToPowerOfTwo( (1ull << 62) + 1) == 1ull << 63);
    CHECK(RoundUpToPowerOfTwo(0xffffffffffffffff) == 0);
    CHECK(RoundUpToPowerOfTwo(0x7fffffffffffffff) == 0x8000000000000000);
}

TEST_CASE("ReverseBitsInBytes") {
    AreYouFastEnough _;
    CHECK(ReverseBitsInBytes(0x0123456789abcdef) == 0x80c4a2e691d5b3f7);
    CHECK(ReverseBitsInBytes(0x0000000000000000) == 0x0000000000000000);
    CHECK(ReverseBitsInBytes(0xffffffffffffffff) == 0xffffffffffffffff);
    CHECK(ReverseBitsInBytes(0x0102030405060708) == 0x8040c020a060e010);
    CHECK(ReverseBitsInBytes(0xff00ff00ff00ff00) == 0xff00ff00ff00ff00);
}

TEST_CASE("AlignDown") {
    CHECK(AlignDown(0, 4) == 0);
    CHECK(AlignDown(1, 4) == 0);
    CHECK(AlignDown(2, 4) == 0);
    CHECK(AlignDown(3, 4) == 0);
    CHECK(AlignDown(4, 4) == 4);
    CHECK(AlignDown(5, 4) == 4);
    CHECK(AlignDown(0, 8) == 0);
    CHECK(AlignDown(1, 8) == 0);
    CHECK(AlignDown(0, 4096) == 0);
    CHECK(AlignDown(0xffffffffffffffff, 4096) == 0xfffffffffffff000);
}

TEST_CASE("AlignUp") {
    CHECK(AlignUp(0, 4) == 0);
    CHECK(AlignUp(1, 4) == 4);
    CHECK(AlignUp(2, 4) == 4);
    CHECK(AlignUp(3, 4) == 4);
    CHECK(AlignUp(4, 4) == 4);
    CHECK(AlignUp(5, 4) == 8);
    CHECK(AlignUp(0, 8) == 0);
    CHECK(AlignUp(1, 8) == 8);
    CHECK(AlignUp(0, 4096) == 0);
    CHECK(AlignUp(1, 4096) == 0x1000);
    CHECK(AlignUp(0xffffffffffffefff, 4096) == 0xfffffffffffff000);
    CHECK(AlignUp(0xffffffffffffffff, 4096) == 0x0);
}
