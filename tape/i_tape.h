#pragma once

#include <cstdint>

class i_tape {  // TODO: abstract_tape
   public:
    constexpr static std::int32_t DEFAULT_VALUE = 0;

    virtual std::int32_t get() const = 0;
    virtual void put(int x) = 0;

    virtual ~i_tape() = default;

    void left();
    void right();

    // Swap semantics for left & right. Current cell remains the same.
    void flip();  // TODO: reset
    void skip_n(std::int64_t delta);

   protected:
    virtual void left_impl() = 0;
    virtual void right_impl() = 0;

   private:
    bool is_flipped{false};
};
