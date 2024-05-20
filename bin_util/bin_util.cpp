#include "bin_util.h"

#include <cstdint>
#include <functional>

namespace {
void copy(std::istream& in,
          std::ostream& out,
          std::function<std::int32_t()> read,
          std::function<void(std::int32_t)> write);
}  // namespace

void from_binary(std::istream& in, std::ostream& out) {
    auto read = [&] {
        std::int32_t x;
        in.read(reinterpret_cast<char*>(&x), sizeof(x));
        return x;
    };
    auto write = [&](std::int32_t x) {
        out << x << ' ';
    };

    copy(in, out, read, write);
}

void to_binary(std::istream& in, std::ostream& out) {
    auto read = [&] {
        std::int32_t x;
        in >> x;
        return x;
    };
    auto write = [&](std::int32_t x) {
        out.write(reinterpret_cast<const char*>(&x), sizeof(x));
    };

    copy(in, out, read, write);
}

namespace {
void copy(std::istream& in,
          std::ostream& out,
          std::function<std::int32_t()> read,
          std::function<void(std::int32_t)> write) {
    while (in) {
        std::int32_t x = read();
        if (in.bad()) {
            throw std::runtime_error("read failure");
        }
        if (in.eof()) {
            break;
        }
        write(x);
        if (out.bad()) {
            throw std::runtime_error("write failure");
        }
    }
}
}  // namespace
