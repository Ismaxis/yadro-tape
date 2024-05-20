#include <gtest/gtest.h>
#include <inmemory/inmemory_tape.h>
#include <tape.h>

#include "test_util.h"

TEST(inmemory_tape, basic) {
    inmemory_tape t;
    ASSERT_EQ(tape::DEFAULT_VALUE, t.get());

    t.put(123);
    ASSERT_EQ(123, t.get());

    t.left();
    ASSERT_EQ(tape::DEFAULT_VALUE, t.get());

    t.put(124);
    ASSERT_EQ(124, t.get());

    t.right();
    ASSERT_EQ(123, t.get());

    t.right();
    ASSERT_EQ(tape::DEFAULT_VALUE, t.get());

    t.put(125);
    ASSERT_EQ(125, t.get());
}

TEST(inmemory_tape, sequence) {
    auto size = 10;
    std::vector<std::int32_t> seq = test_util::get_random_vec(size);

    inmemory_tape t;
    for (auto &&i : seq) {
        t.put(i);
        t.right();
    }

    for (size_t i = size - 1; i < size; i--) {
        t.left();
        ASSERT_EQ(seq[i], t.get());
    }
}
