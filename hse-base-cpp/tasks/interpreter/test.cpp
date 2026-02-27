#include <catch.hpp>

#include "interpreter.h"

namespace {

constexpr size_t kDefaultTapeSize = 30000;

std::string Run(std::string_view code, std::string_view input = "") {
    return Interpreter(kDefaultTapeSize).Run(code, input);
}

}  // namespace

TEST_CASE("Empty program") {
    CHECK(Run("") == "");
    CHECK(Run("", "hello") == "");
}

TEST_CASE("Comments are ignored") {
    CHECK(Run("this is not a real program") == "");
    CHECK(Run("no ops here! just text 123", "input") == "");
}

TEST_CASE("Single dot outputs zero byte") {
    std::string result = Run(".");
    REQUIRE(result.size() == 1);
    CHECK(result[0] == '\0');
}

TEST_CASE("Increment and output") {
    SECTION("Single +") {
        std::string result = Run("+.");
        REQUIRE(result.size() == 1);
        CHECK(result[0] == 1);
    }

    SECTION("Character A") {
        // 'A' = 65
        std::string code(65, '+');
        code += '.';
        std::string result = Run(code);
        REQUIRE(result.size() == 1);
        CHECK(result[0] == 'A');
    }
}

TEST_CASE("Decrement") {
    SECTION("Underflow wraps around") {
        // 0 - 1 = 255
        std::string result = Run("-.");
        REQUIRE(result.size() == 1);
        CHECK(static_cast<uint8_t>(result[0]) == 255);
    }

    SECTION("Increment then decrement") {
        std::string result = Run("+++--.");
        REQUIRE(result.size() == 1);
        CHECK(result[0] == 1);
    }
}

TEST_CASE("Overflow wraps around") {
    // 255 + 1 = 0
    std::string code(256, '+');
    code += '.';
    std::string result = Run(code);
    REQUIRE(result.size() == 1);
    CHECK(result[0] == '\0');
}

TEST_CASE("Input") {
    SECTION("Read single character") {
        CHECK(Run(",.", "A") == "A");
    }

    SECTION("Read multiple characters") {
        CHECK(Run(",.,.,.", "Hey") == "Hey");
    }

    SECTION("Input exhausted gives zero") {
        std::string result = Run(",,.", "X");
        REQUIRE(result.size() == 1);
        CHECK(result[0] == '\0');
    }
}

TEST_CASE("Pointer movement") {
    SECTION("Write to different cells") {
        // cell[0] = 1, cell[1] = 2, output cell[1] then cell[0]
        std::string result = Run("+>++.<.");
        REQUIRE(result.size() == 2);
        CHECK(result[0] == 2);
        CHECK(result[1] == 1);
    }

    SECTION("Move right and left") {
        CHECK(Run(",><.", "H") == "H");
    }
}

TEST_CASE("Simple loops") {
    SECTION("Skip loop when cell is zero") {
        CHECK(Run("[++++.]") == "");
    }

    SECTION("Loop runs until cell is zero") {
        // cell = 3, loop: output and decrement
        std::string result = Run("+++[.-]");
        REQUIRE(result.size() == 3);
        CHECK(result[0] == 3);
        CHECK(result[1] == 2);
        CHECK(result[2] == 1);
    }

    SECTION("Copy cell value") {
        // cell[0] = 5, copy to cell[1]: [->+<]
        std::string result = Run("+++++[->+<]>.");
        REQUIRE(result.size() == 1);
        CHECK(result[0] == 5);
    }
}

TEST_CASE("Nested loops") {
    SECTION("Multiplication: 3 * 4 = 12") {
        std::string result = Run("+++[>++++<-]>.");
        REQUIRE(result.size() == 1);
        CHECK(result[0] == 12);
    }

    SECTION("Deeply nested") {
        // cell[0]=2, cell[1]=3 each iteration, cell[2] accumulates
        // result: cell[2] = 2*3 = 6
        std::string result = Run("++[>+++[>+<-]<-]>>.");
        REQUIRE(result.size() == 1);
        CHECK(result[0] == 6);
    }
}

TEST_CASE("Cat program") {
    CHECK(Run(",[.,]", "Hello!") == "Hello!");
    CHECK(Run(",[.,]") == "");
    CHECK(Run(",[.,]", "A") == "A");
}

TEST_CASE("Hello World") {
    std::string code =
        "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]"
        ">>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    CHECK(Run(code) == "Hello World!\n");
}

TEST_CASE("Addition of two digits") {
    // a + b - '0': because each char already contains '0', the sum has one extra.
    std::string code =
        ","                            // read first digit to cell[0]
        ">,"                           // read second digit to cell[1]
        "[<+>-]"                       // add cell[1] to cell[0]
        "<>++++++[<-------->-]<"       // subtract 48 ('0')
        ".";

    CHECK(Run(code, "35") == "8");
    CHECK(Run(code, "00") == "0");
    CHECK(Run(code, "11") == "2");
}

TEST_CASE("Caesar cipher") {
    // Add 1 to each character: HAL -> IBM
    CHECK(Run(",[+.,]", "HAL") == "IBM");
}

TEST_CASE("Tape persists between runs") {
    Interpreter interp(30000);

    // First run: write 'H' to cell[0], 'i' to cell[1]
    interp.Run(",>,", "Hi");

    // Second run: pointer resets to cell[0], but data stays
    CHECK(interp.Run(".>.", "") == "Hi");

    // Third run: modify cell[0] and read it
    CHECK(interp.Run("+.", "") == "I");  // 'H' + 1 = 'I'
}

TEST_CASE("Small tape") {
    Interpreter interp(5);

    // Use all 5 cells
    std::string result = interp.Run("+>++>+++>++++>+++++.", "");
    REQUIRE(result.size() == 1);
    CHECK(result[0] == 5);
}

TEST_CASE("Large program stress test") {
    SECTION("Long sequence of increments") {
        std::string code(1000, '+');
        code += '.';
        std::string result = Run(code);
        REQUIRE(result.size() == 1);
        CHECK(static_cast<uint8_t>(result[0]) == static_cast<uint8_t>(1000 % 256));
    }

    SECTION("Many iterations") {
        // Count down from 255: outputs 255 characters
        std::string result = Run("-[.-]");
        CHECK(result.size() == 255);
    }
}
