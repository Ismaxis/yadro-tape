#include "tape.h"
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
}  // namespace inmemory
