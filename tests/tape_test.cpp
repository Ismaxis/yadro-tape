#include <gtest/gtest.h>
#include <i_tape.h>
#include <inmemory/tape.h>

#include "test_util.h"

using tape = inmemory::tape;

TEST(inmemory, basic) {
    tape t;
    ASSERT_EQ(i_tape::DEFAULT_VALUE, t.get());

    t.put(123);
    ASSERT_EQ(123, t.get());

    t.left();
    ASSERT_EQ(i_tape::DEFAULT_VALUE, t.get());

    t.put(124);
    ASSERT_EQ(124, t.get());

    t.right();
    ASSERT_EQ(123, t.get());

    t.right();
    ASSERT_EQ(i_tape::DEFAULT_VALUE, t.get());

    t.put(125);
    ASSERT_EQ(125, t.get());
}

TEST(inmemory, sequence) {
    auto size = 10;
    std::vector<std::int32_t> seq = test_util::get_random_vec(size);

    tape t;
    for (auto &&i : seq) {
        t.put(i);
        t.right();
    }

    for (size_t i = size - 1; i < size; i--) {
        t.left();
        ASSERT_EQ(seq[i], t.get());
    }
}
