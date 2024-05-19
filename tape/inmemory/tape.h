#pragma once
#include <cstdint>
#include <deque>

#include "../i_tape.h"

namespace inmemory {
class tape : public i_tape {
   private:
    bool flipped{false};

    std::deque<std::int32_t> q{DEFAULT_VALUE};
    std::size_t index{0};

   public:
    int get() const override;
    void put(int x) override;
    void left() override;
    void right() override;
    // void print() override;
    void flip() override;
    void skip_n(std::int64_t delta) override;
};
}  // namespace inmemory
