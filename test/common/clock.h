//
// Created by konstantin on 11.05.24.
//

#pragma once

#include <chrono>

namespace common {

class ProcessCPUTimer {
public:
    ProcessCPUTimer() { Reset(); }

    std::chrono::microseconds Spent() const {
        return std::chrono::microseconds(SpentMicros());
    }

    std::chrono::microseconds Elapsed() const { return Spent(); }

    void Reset() { start_ts_ = std::clock(); }

private:
    std::clock_t start_ts_;

    size_t SpentMicros() const {
        const size_t clocks = std::clock() - start_ts_;
        return ClocksToMicros(clocks);
    }

    static size_t ClocksToMicros(const size_t clocks) {
        return (clocks * 1'000'000) / CLOCKS_PER_SEC;
    }
};

class ThreadCPUTimer {
public:
    ThreadCPUTimer() { Reset(); }

    std::chrono::nanoseconds Spent() const {
        return std::chrono::nanoseconds(SpentNanos());
    }

    // Backward compatibility
    std::chrono::nanoseconds Elapsed() const { return Spent(); }

    void Reset() { clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_); }

private:
    struct timespec start_;

    uint64_t SpentNanos() const {
        struct timespec now;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);

        return ToNanos(now) - ToNanos(start_);
    }

    static uint64_t ToNanos(const struct timespec& tp) {
        return tp.tv_sec * 1'000'000'000 + tp.tv_nsec;
    }
};

template <typename Clock = std::chrono::steady_clock>
class StopWatch {
    using TimePoint = typename Clock::time_point;
    using Duration = typename Clock::duration;

public:
    StopWatch() : start_(Now()) {
        static_assert(Clock::is_steady, "Steady clock required");
    }

    Duration Elapsed() const {
        return Now() - start_;
    }

    Duration Restart() {
        auto elapsed = Elapsed();
        start_ = Now();
        return elapsed;
    }

private:
    static TimePoint Now() {
        return Clock::now();
    }

private:
    TimePoint start_;
};


}  // namespace common
