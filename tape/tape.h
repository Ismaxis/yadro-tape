#pragma once

#include <cstdint>

class tape {  // TODO: abstract_tape
   public:
    constexpr static std::int32_t DEFAULT_VALUE = 0;

    virtual ~tape() = default;

    virtual std::int32_t get() const = 0;
    virtual void put(int x) = 0;
    virtual void left() = 0;
    virtual void right() = 0;
    virtual void reset() = 0;
};
