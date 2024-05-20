#include <file/signle_direction_tape.h>
#include <gtest/gtest.h>
#include <i_tape.h>

#include "test_util.h"

using tape = file::signle_direction_tape;

auto size = 10240;

TEST(file, right) {
    test_util::test_random_vec(size, [](auto seq) {
        {
            tape t("tmp");
            for (auto &&i : seq) {
                t.put(i);
                t.right();
            }
        }

        std::fstream f("tmp", std::ios::in | std::ios::binary);
        std::vector<int32_t> v;
        bin_util::read_vector(f, seq.size(), v);

        ASSERT_EQ(seq, v);
    });
}

TEST(file, left) {
    test_util::test_random_vec(size, [](auto seq) {
        {
            tape t("tmp");
            for (auto &&i : seq) {
                t.right();
            }
            for (auto &&i : seq) {
                t.left();
                t.put(i);
            }
        }

        std::fstream f("tmp", std::ios::in | std::ios::binary);
        std::vector<int32_t> v;
        bin_util::read_vector(f, seq.size(), v);
        std::ranges::reverse(v);
        ASSERT_EQ(seq, v);
    });
}

TEST(file, right_and_left) {
    test_util::test_random_vec(size, [](auto seq) {
        tape t("tmp");
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

TEST(file, flip) {
    test_util::test_random_vec(size, [](auto seq) {
        tape t("tmp");
        for (auto &&i : seq) {
            t.put(i);
            t.right();
        }

        std::ranges::reverse(seq);

        t.flip();
        for (auto &&i : seq) {
            t.right();
            ASSERT_EQ(i, t.get());
        }
    });
}
