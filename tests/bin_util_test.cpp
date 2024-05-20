#include <bin_util.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <sstream>
#include <vector>

#include "test_util.h"

TEST(bin_util, to_from_binary) {
    test_util::test_random_vec(10, [](std::vector<std::int32_t> v1) {
        std::vector<std::int32_t> v2;

        std::stringstream in;
        std::stringstream bin(std::stringstream::in | std::stringstream::out);
        std::stringstream out;

        std::ranges::copy(v1, std::ostream_iterator<std::int32_t>(in, " "));

        bin_util::to_binary(in, bin);
        bin_util::from_binary(bin, out);

        std::ranges::copy_n(std::istream_iterator<std::int32_t>(out), v1.size(), std::back_inserter(v2));

        ASSERT_EQ(v1, v2);
    });
}

TEST(bin_util, read_write_binary) {
    test_util::test_random_vec(10, [](std::vector<std::int32_t> v1) {
        std::vector<std::int32_t> v2;

        std::stringstream bin(std::stringstream::in | std::stringstream::out);

        bin_util::write_vector(v1, bin);
        bin_util::read_vector(bin, v1.size(), v2);

        ASSERT_EQ(v1, v2);
    });
}
