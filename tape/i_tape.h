#pragma once

#include <cstdint>

class i_tape {
   public:
    constexpr static std::int32_t DEFAULT_VALUE = 0;

    virtual std::int32_t get() const = 0;
    virtual void put(int x) = 0;

    virtual ~i_tape() = default;

    void left();
    void right();
    void flip();
    void skip_n(std::int64_t delta);

    // i_tape& operator++() {
    //     right();
    //     return *this;
    // }

    // i_tape& operator--() {
    //     left();
    //     return *this;
    // }

    // std::int32_t operator*() const {
    //     return get();
    // }

   protected:
    virtual void left_impl() = 0;
    virtual void right_impl() = 0;

   private:
    bool flipped{false};
};
