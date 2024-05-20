#include "inmemory_tape.h"

std::int32_t inmemory_tape::get() const {
    return q[index];
}

void inmemory_tape::put(std::int32_t x) {
    q[index] = x;
}

void inmemory_tape::left() {
    if (index == 0) {
        q.push_front(tape::DEFAULT_VALUE);
    } else {
        --index;
    }
}

void inmemory_tape::right() {
    ++index;
    if (index >= q.size()) {
        q.push_back(tape::DEFAULT_VALUE);
    }
}

void inmemory_tape::reset() {
    index = 0;
}
