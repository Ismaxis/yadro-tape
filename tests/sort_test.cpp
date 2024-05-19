#include <gtest/gtest.h>
#include <i_tape.h>
#include <inmemory/tape.h>
#include <tape_sort.h>

#include "util.h"

using tape = inmemory::tape;

constexpr auto factory = [] { return std::make_unique<tape>(); };

TEST(sort, empty) {
    tape input;
    tape output;

    tape_sort(input, 0, output, factory);

    ASSERT_EQ(i_tape::DEFAULT_VALUE, input.get());
    ASSERT_EQ(i_tape::DEFAULT_VALUE, output.get());
}

TEST(sort, single) {
    tape input;
    tape output;

    input.put(1);

    tape_sort(input, 1, output, factory);

    ASSERT_EQ(1, output.get());
}

void test_on_seq(std::vector<std::int32_t> seq) {
    tape input;
    tape output;

    auto size = seq.size();
    for (size_t i = 0; i < size - 1; i++) {
        input.put(seq[i]);
        input.right();
    }
    input.put(seq[size - 1]);
    input.flip();

    tape_sort(input, size, output, factory);

    std::ranges::sort(seq);

    for (auto &&i : seq) {
        ASSERT_EQ(i, output.get());
        output.right();
    }
    ASSERT_EQ(i_tape::DEFAULT_VALUE, output.get());
}

TEST(sort, sorted) {
    test_on_seq({1, 2, 3, 4});
    test_on_seq({-111, -222, 0, 444, 555});
}

TEST(sort, random) {
    std::size_t max_size = 40;
    std::mt19937 random{std::random_device{}()};
    std::uniform_int_distribution<std::int32_t> dist(1, max_size);
    for (size_t test_case = 0; test_case < 10; test_case++) {
        std::size_t size = dist(random);
        auto seq = util::get_random_vec(size);
        test_on_seq(seq);
    }
}
