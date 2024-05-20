#include "tape.h"
namespace inmemory {
int tape::get() const {
    return q[index];
}

void tape::put(int x) {
    q[index] = x;
}

void tape::left_impl() {
    if (index == 0) {
        q.push_front(i_tape::DEFAULT_VALUE);
    } else {
        --index;
    }
}

void tape::right_impl() {
    ++index;
    if (index >= q.size()) {
        q.push_back(i_tape::DEFAULT_VALUE);
    }
}

}  // namespace inmemory
