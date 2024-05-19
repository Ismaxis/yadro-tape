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

    std::mt19937 random{std::random_device{}()};
    std::uniform_int_distribution<std::int32_t> dist(1, 40);
    for (size_t test_case = 0; test_case < 10; test_case++) {
        std::size_t size = dist(random);
        auto seq = util::get_random_vec(size);

        for (size_t i = 0; i < size; i++) {
            input.put(seq[i]);
            input.right();
        }
        // TODO: input.flip();
        for (size_t i = 0; i < size; i++) {
            input.left();
        }

        tape_sort(input, size, output, [] { return std::make_unique<tape>(); });

        std::ranges::sort(seq);

        for (auto &&i : seq) {
            ASSERT_EQ(i, output.get());
            output.right();
        }
        ASSERT_EQ(i_tape::DEFAULT_VALUE, output.get());
    }
}
