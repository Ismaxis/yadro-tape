#include "i_tape.h"

void i_tape::left() {
    if (is_flipped) {
        right_impl();
    } else {
        left_impl();
    }
}

void i_tape::right() {
    if (is_flipped) {
        left_impl();
    } else {
        right_impl();
    }
}

void i_tape::flip() {
    is_flipped = !is_flipped;
}

void i_tape::skip_n(std::int64_t delta) {
    if (delta >= 0) {
        for (size_t i = 0; i < delta; i++) {
            right();
        }
    } else {
        for (size_t i = 0; i < -delta; i++) {
            left();
        }
    }
}
