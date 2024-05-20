#include "bin_util.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iterator>

namespace bin_util {
namespace {
void copy(std::istream& in,
          std::ostream& out,
          std::function<std::int32_t(std::istream&)> read,
          std::function<void(std::ostream&, std::int32_t)> write);

auto _read_binary = [](std::istream& in) {
    std::int32_t x;
    in.read(reinterpret_cast<char*>(&x), sizeof(x));
    return x;
};
auto _write_binary = [](std::ostream& out, std::int32_t x) {
    out.write(reinterpret_cast<const char*>(&x), sizeof(x));
};
}  // namespace

void from_binary(std::istream& in, std::ostream& out) {
    auto write = [](std::ostream& out, std::int32_t x) {
        out << x << ' ';
    };

    copy(in, out, _read_binary, write);
}

void to_binary(std::istream& in, std::ostream& out) {
    auto read = [](std::istream& in) {
        std::int32_t x;
        in >> x;
        return x;
    };

    copy(in, out, read, _write_binary);
}

void read_vector(std::istream& in, std::size_t n, std::vector<std::int32_t>& v) {
    if (v.size() < n) {
        v.resize(n);
    }
    in.read(reinterpret_cast<char*>(v.data()), sizeof(std::int32_t) * n);
}

void write_vector(std::vector<std::int32_t>& v, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(v.data()), sizeof(std::int32_t) * v.size());
}

namespace {
void copy(std::istream& in,
          std::ostream& out,
          std::function<std::int32_t(std::istream&)> read,
          std::function<void(std::ostream&, std::int32_t)> write) {
    while (in) {
        std::int32_t x = read(in);
        if (in.bad()) {
            throw std::runtime_error("read failure");
        }
        if (in.eof()) {
            break;
        }
        write(out, x);
        if (out.bad()) {
            throw std::runtime_error("write failure");
        }
    }
}
}  // namespace
}  // namespace bin_util
