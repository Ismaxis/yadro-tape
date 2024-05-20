#pragma once

#include <cstdint>

class tape {
   public:
    constexpr static std::int32_t DEFAULT_VALUE = 0;

    virtual ~tape() = default;

    virtual std::int32_t get() const = 0;
    virtual void put(std::int32_t x) = 0;
    virtual void left() = 0;
    virtual void right() = 0;
    virtual void reset() = 0;
};

struct tape_delays {
    std::int64_t get_delay{};
    std::int64_t put_delay{};
    std::int64_t left_delay{};
    std::int64_t right_delay{};
    std::int64_t reset_delay{};
};
