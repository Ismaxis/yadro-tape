#include "tape_sort.h"

#include <algorithm>

void tape_sort(i_tape* input, std::size_t n, i_tape* output) {
    std::vector<std::int32_t> vec(n);
    for (size_t i = 0; i < n; i++) {
        vec[i] = input->get();
        input->right();
    }

    std::ranges::sort(vec);

    for (size_t i = 0; i < n; i++) {
        output->put(vec[i]);
        output->right();
    }
    for (size_t i = 0; i < n; i++) {
        output->left();
    }
}