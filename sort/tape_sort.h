#pragma once
#include <i_tape.h>

#include <functional>
#include <memory>

void tape_sort(
    i_tape& input,
    std::size_t n,
    i_tape& output,
    std::function<std::unique_ptr<i_tape>()> tape_factory);
