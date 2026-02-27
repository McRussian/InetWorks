#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>

class TimeLimiter {
public:
    explicit TimeLimiter(std::chrono::milliseconds cpu_limit)
        : start_cpu_(std::clock())
        , start_wall_(std::chrono::steady_clock::now()) {
        auto wall_kill = cpu_limit * 10;
        watchdog_ = std::thread([this, wall_kill]() {
            std::unique_lock lock(mutex_);
            if (!cv_.wait_for(lock, wall_kill, [this] { return done_; })) {
                std::fprintf(stderr, "\nTIME LIMIT EXCEEDED (wall time)\n");
                std::_Exit(1);
            }
        });
    }

    ~TimeLimiter() {
        {
            std::lock_guard lock(mutex_);
            done_ = true;
        }
        cv_.notify_one();
        watchdog_.join();
    }

    std::chrono::microseconds ElapsedCPUTime() const {
        auto elapsed = std::clock() - start_cpu_;
        return std::chrono::microseconds(elapsed * 1'000'000 / CLOCKS_PER_SEC);
    }

    std::chrono::microseconds ElapsedWallTime() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - start_wall_);
    }

    TimeLimiter(const TimeLimiter&) = delete;
    TimeLimiter& operator=(const TimeLimiter&) = delete;

private:
    std::clock_t start_cpu_;
    std::chrono::steady_clock::time_point start_wall_;

    std::mutex mutex_;
    std::condition_variable cv_;
    bool done_ = false;
    std::thread watchdog_;
};
