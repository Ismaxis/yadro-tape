#include "tape.h"

#include <iostream>  // TODO: remove
namespace inmemory {
int tape::get() const {
    return q[index];
}
void tape::put(int x) {
    q[index] = x;
}
void tape::left() {
    if (index == 0) {
        q.push_front(DEFAULT_VALUE);
    } else {
        --index;
    }
}
void tape::right() {
    ++index;
    if (index >= q.size()) {
        q.push_back(DEFAULT_VALUE);
    }
}
void tape::print() {
    for (size_t i = 0; i < q.size(); i++) {
        if (i == index) {
            std::cout << '[' << q[i] << "] ";
        } else {
            std::cout << q[i] << ' ';
        }
    }
    std::cout << '\n';
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
