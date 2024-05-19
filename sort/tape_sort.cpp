#include "tape_sort.h"

#include <algorithm>
#include <iostream>  // TODO: remove

namespace {
void copy_n(i_tape& from, size_t n, i_tape& to);

void merge(i_tape& buff_0, i_tape& buff_1, i_tape& ptr_0, size_t n1, size_t n2);
}  // namespace

void tape_sort(i_tape& input, std::size_t n, i_tape& output) {
    std::vector<std::int32_t> vec(n);
    for (size_t i = 0; i < n; i++) {
        vec[i] = input.get();
        input.right();
    }

    std::ranges::sort(vec);

    for (size_t i = 0; i < n; i++) {
        output.put(vec[i]);
        output.right();
    }
    for (size_t i = 0; i < n; i++) {
        output.left();
    }
}

void tape_sort(i_tape& input, std::size_t n, i_tape& output,
               std::function<std::unique_ptr<i_tape>()> tape_factory) {
    auto buff_0 = tape_factory();
    auto buff_1 = tape_factory();
    auto ptr_0 = tape_factory();

    auto _print = [&] {
        std::cout << "input\n";
        input.print();
        std::cout << "output0\n";
        output.print();
        std::cout << "buff_0\n";
        buff_0->print();
        std::cout << "buff_1\n";
        buff_1->print();
        std::cout << "ptr_0\n";
        ptr_0->print();
        std::cout << "\n";
    };
    for (size_t i = 0; i < n; i++) {
        buff_0->put(input.get());
        input.right();
        buff_0->right();
    }
    // TODO: ??? flip
    for (size_t i = 0; i < n; i++) {
        buff_0->left();
    }
    // _print();

    bool turn = true;
    for (size_t chunk = 1; chunk < n; chunk *= 2) {
        for (size_t l = 0; l < n; l += 2 * chunk) {
            size_t m = std::min(l + chunk - 1, n - 1);
            size_t r = std::min(l + 2 * chunk - 1, n - 1);

            size_t n1 = m - l + 1;
            size_t n2 = r - m;
            if (turn) {
                ::merge(*buff_0, *buff_1, *ptr_0, n1, n2);
            } else {
                ::merge(*buff_1, *buff_0, *ptr_0, n1, n2);
            }
        }
        turn = !turn;
        buff_0->skip_n(-n);
        buff_1->skip_n(-n);
        _print();
    }

    if (turn) {
        ::copy_n(*buff_0, n, output);
    } else {
        ::copy_n(*buff_1, n, output);
    }
    output.skip_n(-n);
    _print();
}

namespace {

// TODO: iterators
void copy_n(i_tape& from, size_t n, i_tape& to) {
    for (size_t i = 0; i < n; i++) {
        to.put(from.get());
        from.right();
        to.right();
    }
}

void merge(i_tape& buff_0, i_tape& buff_1, i_tape& ptr_0, size_t n1, size_t n2) {
    auto _print = [&] {
        std::cout << "buff_0\n";
        buff_0.print();
        std::cout << "buff_1\n";
        buff_1.print();
        std::cout << "ptr_0\n";
        ptr_0.print();
        std::cout << "\n";
    };

    _print();
    for (size_t i = 0; i < n1; i++) {
        ptr_0.put(buff_0.get());
        ptr_0.right();
        buff_0.right();
    }

    _print();
    ptr_0.skip_n(-n1);

    _print();
    size_t i = 0;
    size_t j = 0;
    while (i < n1 && j < n2) {
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
        _print();
    }

    while (i < n1) {
        buff_1.put(ptr_0.get());
        buff_1.right();
        ptr_0.right();
        i++;
    }
    _print();
    while (j < n2) {
        buff_1.put(buff_0.get());
        buff_1.right();
        buff_0.right();
        j++;
    }

    ptr_0.skip_n(-n1);  // TODO: flip
    _print();
}
}  // namespace