#include "game_of_life.h"
#include "patterns.h"

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

namespace {

volatile std::sig_atomic_t g_interrupted = 0;

void OnSigint(int) {
    g_interrupted = 1;
}

}  // namespace

#ifdef _WIN32
#include <conio.h>

bool KeyPressed() {
    return _kbhit();
}

char ReadKey() {
    return _getch();
}

struct RawMode {};

#else

#include <poll.h>
#include <termios.h>
#include <unistd.h>

struct RawMode {
    termios old_;

    RawMode() {
        tcgetattr(STDIN_FILENO, &old_);
        termios raw = old_;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }

    ~RawMode() {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_);
    }

    RawMode(const RawMode&) = delete;
    RawMode& operator=(const RawMode&) = delete;
};

bool KeyPressed() {
    pollfd pfd{STDIN_FILENO, POLLIN, 0};
    return poll(&pfd, 1, 0) > 0;
}

char ReadKey() {
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) < 0) {
        return 0;
    }
    return c;
}

#endif

// ===== Pattern selection =====

Grid SelectPattern(const std::string& name) {
    if (name == "block") {
        return patterns::Block();
    }
    if (name == "beehive") {
        return patterns::Beehive();
    }
    if (name == "blinker") {
        return patterns::Blinker();
    }
    if (name == "toad") {
        return patterns::Toad();
    }
    if (name == "pulsar") {
        return patterns::Pulsar();
    }
    if (name == "pentadecathlon") {
        return patterns::Pentadecathlon();
    }
    if (name == "glider") {
        return patterns::Glider();
    }
    if (name == "lwss") {
        return patterns::LWSS();
    }
    if (name == "gun") {
        return patterns::GosperGliderGun();
    }
    if (name == "rpentomino") {
        return patterns::RPentomino();
    }
    if (name == "random") {
        return patterns::Random();
    }
    return {};
}

void PrintUsage(const char* prog) {
    std::cerr << "Usage: " << prog << " <pattern> [speed_ms]\n\n"
              << "Patterns:\n"
              << "  block, beehive          Still lifes\n"
              << "  blinker, toad           Period 2 oscillators\n"
              << "  pulsar                  Period 3 oscillator\n"
              << "  pentadecathlon          Period 15 oscillator\n"
              << "  glider, lwss            Spaceships\n"
              << "  gun                     Gosper glider gun\n"
              << "  rpentomino              Chaotic long-lived pattern\n"
              << "  random                  Random 32x32 field\n\n"
              << "Controls:\n"
              << "  Space  pause/resume\n"
              << "  +/-    change speed\n"
              << "  Q      quit\n";
}

int main(int argc, char* argv[]) {
    std::string pattern_name = argc > 1 ? argv[1] : "";
    int delay_ms = argc > 2 ? std::atoi(argv[2]) : 100;

    if (pattern_name.empty()) {
        PrintUsage(argv[0]);
        return 1;
    }

    Grid initial = SelectPattern(pattern_name);
    if (initial.empty()) {
        std::cerr << "Unknown pattern: " << pattern_name << "\n";
        PrintUsage(argv[0]);
        return 1;
    }

    GameOfLife game(initial);

    std::signal(SIGINT, OnSigint);

    RawMode raw;
    std::cout << "\033[?25l";  // Hide cursor
    std::cout << "\033[2J";    // Clear screen

    bool paused = false;
    uint64_t generation = 0;

    while (!g_interrupted) {
        // Handle input
        while (KeyPressed()) {
            char key = ReadKey();
            if (key == ' ') {
                paused = !paused;
            } else if (key == '+' || key == '=') {
                delay_ms = std::max(10, delay_ms - 20);
            } else if (key == '-') {
                delay_ms = std::min(1000, delay_ms + 20);
            } else if (key == 'q' || key == 'Q') {
                g_interrupted = 1;
            }
        }

        std::cout << "\033[H";

        size_t alive = 0;
        std::cout << "Generation: " << generation
                  << " | " << game.Rows() << "x" << game.Cols()
                  << " | Speed: " << (1000 / std::max(1, delay_ms)) << " gen/s"
                  << (paused ? " | PAUSED" : "        ") << "    \n";

        for (size_t r = 0; r < game.Rows(); ++r) {
            for (size_t c = 0; c < game.Cols(); ++c) {
                if (game.IsAlive(r, c)) {
                    std::cout << "\033[47m  \033[0m";
                    ++alive;
                } else {
                    std::cout << "\033[48;5;235m  \033[0m";
                }
            }
            std::cout << "\n";
        }

        std::cout << "Alive: " << alive
                  << " | [Space] pause  [+/-] speed  [Q] quit   \n";

        if (!paused) {
            game.Simulate(1);
            ++generation;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }

    // Clean exit: show cursor, RawMode destructor restores terminal
    std::cout << "\033[?25h" << std::endl;
    return 0;
}
