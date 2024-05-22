#pragma once
#include <tape.h>

#include <functional>
#include <memory>

void tape_sort(
    tape& input,
    std::size_t n,
    std::size_t m,
    tape& output,
    std::function<std::unique_ptr<tape>()> tape_factory);
