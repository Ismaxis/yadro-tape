#include <file/tape.h>
#include <gtest/gtest.h>
#include <i_tape.h>

#include "test_util.h"

using tape = file::tape;

// TEST(file, right) {
//     std::fstream f("tmp");
//     tape t(f);
//     ASSERT_EQ(i_tape::DEFAULT_VALUE, t.get());

//     t.put(123);
//     ASSERT_EQ(123, t.get());

//     t.right();
//     ASSERT_EQ(123, t.get());

//     t.right();
//     ASSERT_EQ(i_tape::DEFAULT_VALUE, t.get());

//     t.put(125);
//     ASSERT_EQ(125, t.get());
// }

auto size = 1024;

TEST(file, right) {
    test_util::test_random_vec(size, [](auto seq) {
        std::fstream f("tmp", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
        {
            tape t(f);
            for (auto &&i : seq) {
                t.put(i);
                t.right();
            }
        }

        f.seekg(0, f.beg);
        std::vector<int32_t> v;
        bin_util::read_vector(f, seq.size(), v);

        ASSERT_EQ(seq, v);
    });
}

TEST(file, left) {
    test_util::test_random_vec(size, [](auto seq) {
        std::fstream f("tmp", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
        {
            tape t(f);
            for (auto &&i : seq) {
                t.right();
            }
            for (auto &&i : seq) {
                t.left();
                t.put(i);
            }
        }

        f.seekg(0, f.beg);
        std::vector<int32_t> v;
        bin_util::read_vector(f, seq.size(), v);
        std::ranges::reverse(v);
        ASSERT_EQ(seq, v);
    });
}

TEST(file, right_and_left) {
    test_util::test_random_vec(size, [](auto seq) {
        std::fstream f("tmp", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

        tape t(f);
        for (auto &&i : seq) {
            t.put(i);
            t.right();
        }

        std::ranges::reverse(seq);

        for (auto &&i : seq) {
            t.left();
            ASSERT_EQ(i, t.get());
        }
    });
}
