#include <file/signle_direction_tape.h>
#include <gtest/gtest.h>
#include <i_tape.h>
#include <tape_sort.h>

#include <array>

#include "test_util.h"

using tape = file::signle_direction_tape;

std::array<std::string, 3> files{
    "buff_0",
    "buff_1",
    "ptr_0",
};

constexpr auto factory = [] {
    static std::size_t i = 0;
    if (i == files.size()) {
        i = 0;
    }
    return std::make_unique<tape>(files[i++]);
};

TEST(sort, empty) {
    tape input("in", true);
    tape output("out", true);

    tape_sort(input, 0, output, factory);

    ASSERT_EQ(i_tape::DEFAULT_VALUE, input.get());
    ASSERT_EQ(i_tape::DEFAULT_VALUE, output.get());
}

TEST(sort, single) {
    tape input("in");
    tape output("out");

    input.put(1);

    tape_sort(input, 1, output, factory);

    ASSERT_EQ(1, output.get());
}

void test_on_seq(std::vector<std::int32_t> seq) {
    tape input("in");
    tape output("out");

    auto size = seq.size();
    for (size_t i = 0; i < size; i++) {
        input.right();
        input.put(seq[i]);
    }
    input.flip();

    tape_sort(input, size, output, factory);

    std::ranges::sort(seq);

    for (auto &&i : seq) {
        ASSERT_EQ(i, output.get());
        output.right();
    }
}

TEST(sort, sorted) {
    test_on_seq({1, 2, 3, 4});
    test_on_seq({-111, -222, 0, 444, 555});
}

TEST(sort, random) {
    test_util::test_random_vec(102400, test_on_seq);
}
