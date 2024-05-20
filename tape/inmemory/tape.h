#pragma once
#include <deque>

#include "../i_tape.h"

namespace inmemory {
class tape : public i_tape {
   public:
    int get() const override;
    void put(int x) override;

    virtual ~tape() override = default;

   protected:
    void left_impl() override;
    void right_impl() override;

   private:
    std::deque<std::int32_t> q{DEFAULT_VALUE};
    std::size_t index{0};
};
}  // namespace inmemory
