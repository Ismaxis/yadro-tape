#include <gtest/gtest.h>
#include <i_tape.h>
#include <inmemory/tape.h>
#include <tape_sort.h>

#include "util.h"

using tape = inmemory::tape;

TEST(sort, empty) {
    tape input;
    tape output;

    tape_sort(input, 0, output);

    ASSERT_EQ(i_tape::DEFAULT_VALUE, input.get());
    ASSERT_EQ(i_tape::DEFAULT_VALUE, output.get());
}

TEST(sort, single) {
    tape input;
    tape output;

    input.put(1);

    tape_sort(input, 1, output);

    ASSERT_EQ(1, output.get());
}

TEST(sort, small) {
    tape input;
    tape output;
    std::size_t size = 5;
    auto seq = util::get_random_vec(size);

    for (size_t i = 0; i < size; i++) {
        input.put(seq[i]);
        input.right();
    }
    // input.flip();
    for (size_t i = 0; i < size; i++) {
        input.left();
    }

    tape_sort(input, size, output);

    std::ranges::sort(seq);

    for (auto &&i : seq) {
        std::cout << i << " " << output.get() << '\n';
        ASSERT_EQ(i, output.get());
        output.right();
    }
    ASSERT_EQ(i_tape::DEFAULT_VALUE, output.get());
}
