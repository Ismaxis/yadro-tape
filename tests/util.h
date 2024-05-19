#pragma once
#include <algorithm>
#include <random>
#include <vector>

namespace util {

using int32_limits = std::numeric_limits<std::int32_t>;

inline std::vector<std::int32_t> get_random_vec(
    std::size_t size,
    std::int32_t lower = int32_limits::min(),
    std::int32_t upper = int32_limits::max()) {
    std::mt19937 mersenne_engine{std::random_device{}()};
    std::uniform_int_distribution<std::int32_t> dist(lower, upper);

    std::vector<int> res(size);
    std::ranges::generate(res, [&]() { return dist(mersenne_engine); });
    return res;
}

}  // namespace util
