#pragma once
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>

namespace bin_util {

std::size_t from_binary(std::istream& in, std::ostream& out, std::size_t limit = -1);
std::size_t to_binary(std::istream& in, std::ostream& out, std::size_t limit = -1);

void read_vector(std::istream& in, std::size_t n, std::vector<std::int32_t>& v);
void write_vector(std::vector<std::int32_t>& v, std::ostream& out);

}  // namespace bin_util