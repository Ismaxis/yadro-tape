#pragma once

#include <cstdint>

class i_tape {
   public:
    constexpr static std::int32_t DEFAULT_VALUE = 0;

    virtual int get() const = 0;
    virtual void put(int x) = 0;
    virtual void left() = 0;
    virtual void right() = 0;
    // virtual void flip() = 0; // flips right and left
};
