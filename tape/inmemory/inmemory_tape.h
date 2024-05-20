#pragma once
#include <deque>

#include "../tape.h"

class inmemory_tape : public tape {
   public:
    virtual ~inmemory_tape() override = default;

    std::int32_t get() const override;
    void put(std::int32_t x) override;
    void left() override;
    void right() override;
    void reset() override;

   private:
    std::deque<std::int32_t> q{DEFAULT_VALUE};
    std::size_t index{0};
};
