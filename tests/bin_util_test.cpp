#include <bin_util.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <sstream>
#include <vector>

#include "util.h"

TEST(bin_util, correctness) {
    std::size_t max_size = 10;
    std::mt19937 random{std::random_device{}()};
    std::uniform_int_distribution<std::int32_t> dist(1, max_size);
    for (size_t test_case = 0; test_case < 10; test_case++) {
        std::vector<std::int32_t> v1 = util::get_random_vec(dist(random), 0, 2);
        std::vector<std::int32_t> v2(v1.size());

        std::stringstream in;
        std::stringstream bin(std::stringstream::in | std::stringstream::out);
        std::stringstream out;

        std::ranges::copy(v1, std::ostream_iterator<std::int32_t>(in, " "));

        to_binary(in, bin);
        from_binary(bin, out);

        std::ranges::copy_n(std::istream_iterator<std::int32_t>(out), v1.size(), v2.begin());

        ASSERT_EQ(v1, v2);
    }
}
