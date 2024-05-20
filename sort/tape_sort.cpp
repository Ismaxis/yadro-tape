#include "tape_sort.h"

#include <algorithm>

namespace {
void copy_n(i_tape& from, size_t n, i_tape& to);

void merge(i_tape& buff_0, i_tape& buff_1, i_tape& ptr_0, size_t left_n, size_t right_n);
}  // namespace

void tape_sort(i_tape& input, std::size_t n, i_tape& output,
               std::function<std::unique_ptr<i_tape>()> tape_factory) {
    auto buff_0 = tape_factory();
    auto buff_1 = tape_factory();
    auto ptr_0 = tape_factory();

    ::copy_n(input, n, *buff_0);

    buff_0->skip_n(-n);

    bool turn = true;
    for (size_t chunk = 1; chunk < n; chunk *= 2) {
        for (size_t l = 0; l < n; l += 2 * chunk) {
            size_t m = std::min(l + chunk - 1, n - 1);
            size_t r = std::min(l + 2 * chunk - 1, n - 1);

            size_t left_n = m - l + 1;
            size_t right_n = r - m;
            if (turn) {
                ::merge(*buff_0, *buff_1, *ptr_0, left_n, right_n);
            } else {
                ::merge(*buff_1, *buff_0, *ptr_0, left_n, right_n);
            }
        }
        turn = !turn;
        buff_0->skip_n(-n);
        buff_1->skip_n(-n);
    }

    if (turn) {
        ::copy_n(*buff_0, n, output);
    } else {
        ::copy_n(*buff_1, n, output);
    }
    output.skip_n(-n);
}

namespace {

void copy_n(i_tape& from, size_t n, i_tape& to) {
    for (size_t i = 0; i < n; i++) {
        to.put(from.get());
        from.right();
        to.right();
    }
}

void merge(i_tape& buff_0, i_tape& buff_1, i_tape& ptr_0, size_t left_n, size_t right_n) {
    for (size_t i = 0; i < left_n; i++) {
        ptr_0.put(buff_0.get());
        ptr_0.right();
        buff_0.right();
    }

    ptr_0.skip_n(-left_n);

    size_t i = 0;
    size_t j = 0;
    while (i < left_n && j < right_n) {
        auto l = ptr_0.get();
        auto r = buff_0.get();
        if (l <= r) {
            buff_1.put(l);
            ptr_0.right();
            i++;
        } else {
            buff_1.put(r);
            buff_0.right();
            j++;
        }
        buff_1.right();
    }

    while (i < left_n) {
        buff_1.put(ptr_0.get());
        buff_1.right();
        ptr_0.right();
        i++;
    }
    while (j < right_n) {
        buff_1.put(buff_0.get());
        buff_1.right();
        buff_0.right();
        j++;
    }

    ptr_0.skip_n(-left_n);
}
}  // namespace