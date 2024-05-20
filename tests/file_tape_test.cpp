#include <file/file_tape.h>
#include <gtest/gtest.h>
#include <tape.h>

#include "test_util.h"

auto size = 10240;

TEST(file, right) {
    test_util::test_random_vec(size, [](auto seq) {
        {
            file_tape t("tmp");
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
            file_tape t("tmp");
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
        file_tape t("tmp");
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

TEST(file, reset) {
    test_util::test_random_vec(size, [](auto seq) {
        file_tape t("tmp");
        for (auto &&i : seq) {
            t.put(i);
            t.right();
        }

        t.reset();
        for (auto &&i : seq) {
            ASSERT_EQ(i, t.get());
            t.right();
        }
    });
}
