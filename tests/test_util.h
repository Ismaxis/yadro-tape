#pragma once
#include <algorithm>
#include <random>
#include <vector>

namespace test_util {

using int32_limits = std::numeric_limits<std::int32_t>;

inline std::vector<std::int32_t> get_random_vec(
    std::size_t size,
    std::int32_t lower = int32_limits::min(),
    std::int32_t upper = int32_limits::max()) {
    std::mt19937 random{std::random_device{}()};
    std::uniform_int_distribution<std::int32_t> dist(lower, upper);

    std::vector<int> res(size);
    std::ranges::generate(res, [&]() { return dist(random); });
    return res;
}

inline void test_random_vec(std::size_t max_size, std::function<void(std::vector<std::int32_t>)> test, std::size_t test_cases = 10) {
    std::mt19937 random{std::random_device{}()};
    std::uniform_int_distribution<std::int32_t> dist(1, max_size);
    for (size_t test_case = 0; test_case < test_cases; test_case++) {
        std::vector<std::int32_t> v = test_util::get_random_vec(dist(random));
        test(v);
    }
}

}  // namespace test_util
