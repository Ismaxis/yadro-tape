#include "tape.h"

#include <iostream>  // TODO: remove
namespace inmemory {
int tape::get() const {
    return q[index];
}

void tape::put(int x) {
    q[index] = x;
}

namespace {
void left_impl(std::deque<std::int32_t>& q, std::size_t& index) {
    if (index == 0) {
        q.push_front(i_tape::DEFAULT_VALUE);
    } else {
        --index;
    }
}
void right_impl(std::deque<std::int32_t>& q, std::size_t& index) {
    ++index;
    if (index >= q.size()) {
        q.push_back(i_tape::DEFAULT_VALUE);
    }
}
}  // namespace

void tape::left() {
    if (flipped) {
        inmemory::right_impl(q, index);
    } else {
        inmemory::left_impl(q, index);
    }
}

void tape::right() {
    if (flipped) {
        inmemory::left_impl(q, index);
    } else {
        inmemory::right_impl(q, index);
    }
}

// void tape::print() {
//     for (size_t i = 0; i < q.size(); i++) {
//         if (i == index) {
//             std::cout << '[' << q[i] << "] ";
//         } else {
//             std::cout << q[i] << ' ';
//         }
//     }
//     std::cout << '\n';
// }

void tape::flip() {
    flipped = !flipped;
}

void tape::skip_n(std::int64_t delta) {
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
}  // namespace inmemory
