#pragma once

#include <istream>
#include <ostream>

#include "../i_tape.h"

namespace file {
class tape : public i_tape {
   public:
    tape(std::iostream& in) {
    }

    int get() const override;
    void put(int x) override;

   protected:
    virtual void left_impl() = 0;
    virtual void right_impl() = 0;
};
}  // namespace file
