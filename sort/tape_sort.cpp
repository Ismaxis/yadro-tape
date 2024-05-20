#include "tape_sort.h"

#include <algorithm>

namespace {
void copy_n(tape& from, std::size_t n, tape& to);

void merge(tape& buff_0, tape& buff_1, tape& merge_ptr, std::size_t left_n, std::size_t right_n);
}  // namespace

void tape_sort(tape& input, std::size_t n, tape& output,
               std::function<std::unique_ptr<tape>()> tape_factory) {
    auto buff_0 = tape_factory();
    auto buff_1 = tape_factory();
    auto merge_ptr = tape_factory();

    ::copy_n(input, n, *buff_0);

    buff_0->reset();

    bool turn = true;
    for (std::size_t chunk = 1; chunk < n; chunk *= 2) {
        for (std::size_t left = 0; left < n; left += 2 * chunk) {
            std::size_t middle = std::min(left + chunk - 1, n - 1);
            std::size_t right = std::min(left + 2 * chunk - 1, n - 1);

            std::size_t left_n = middle - left + 1;
            std::size_t right_n = right - middle;
            if (turn) {
                ::merge(*buff_0, *buff_1, *merge_ptr, left_n, right_n);
            } else {
                ::merge(*buff_1, *buff_0, *merge_ptr, left_n, right_n);
            }
        }
        turn = !turn;
        buff_0->reset();
        buff_1->reset();
    }

    if (turn) {
        ::copy_n(*buff_0, n, output);
    } else {
        ::copy_n(*buff_1, n, output);
    }
    output.reset();
}

namespace {
void copy_n(tape& from, std::size_t n, tape& to) {
    for (std::size_t i = 0; i < n; i++) {
        to.put(from.get());
        from.right();
        to.right();
    }
}

void merge(tape& buff_0, tape& buff_1, tape& merge_ptr, std::size_t left_n, std::size_t right_n) {
    ::copy_n(buff_0, left_n, merge_ptr);

    merge_ptr.reset();

    std::size_t i = 0;
    std::size_t j = 0;
    while (i < left_n && j < right_n) {
        auto l = merge_ptr.get();
        auto r = buff_0.get();
        if (l <= r) {
            buff_1.put(l);
            merge_ptr.right();
            i++;
        } else {
            buff_1.put(r);
            buff_0.right();
            j++;
        }
        buff_1.right();
    }

    ::copy_n(merge_ptr, left_n - i, buff_1);
    ::copy_n(buff_0, right_n - j, buff_1);

    merge_ptr.reset();
}
}  // namespace
