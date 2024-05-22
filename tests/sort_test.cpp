#include <file/file_tape.h>
#include <file_util.h>
#include <gtest/gtest.h>
#include <tape.h>
#include <tape_sort.h>

#include <array>
#include <filesystem>

#include "test_util.h"

constexpr auto factory = [] {
    return std::make_unique<file_tape>(file_util::new_tmp_file());
};

TEST(sort, empty) {
    file_tape input("empty_in");
    file_tape output("empty_out");

    tape_sort(input, 0, 1, output, factory);

    ASSERT_EQ(tape::DEFAULT_VALUE, input.get());
    ASSERT_EQ(tape::DEFAULT_VALUE, output.get());
}

TEST(sort, single) {
    file_tape input("in");
    file_tape output("out");

    input.put(1);

    tape_sort(input, 1, 1, output, factory);

    ASSERT_EQ(1, output.get());
}

void test_on_seq(std::vector<std::int32_t> seq) {
    static std::mt19937 random{std::random_device{}()};
    file_tape input("in");
    file_tape output("out");

    auto size = seq.size();
    for (std::size_t i = 0; i < size; i++) {
        input.put(seq[i]);
        input.right();
    }
    input.reset();

    std::uniform_int_distribution<std::size_t> dist(1, 5 * size / 4);

    tape_sort(input, size, dist(random), output, factory);

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
    test_util::test_random_vec(10240, test_on_seq);
}
