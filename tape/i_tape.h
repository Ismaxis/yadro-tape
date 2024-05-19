#pragma once

#include <cstdint>

class i_tape {
   public:
    constexpr static std::int32_t DEFAULT_VALUE = 0;

    virtual std::int32_t get() const = 0;
    virtual void put(int x) = 0;
    virtual void left() = 0;
    virtual void right() = 0;
    // virtual void flip() = 0; // flips right and left
    virtual void print() = 0;
    virtual void skip_n(std::int64_t delta) = 0;
};
