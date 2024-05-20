#include "file_tape.h"

#include <bin_util.h>

#include <chrono>
#include <thread>

namespace {
auto binary_read_write = std::fstream::binary | std::fstream::in | std::fstream::out;
auto binary_create_read_write = binary_read_write | std::fstream::trunc;
using mc = std::chrono::microseconds;
}  // namespace

file_tape::file_tape(std::filesystem::path filepath, tape_delays delays)
    : delays(delays),
      buff(BUFF_SIZE, DEFAULT_VALUE),
      prev_buff(BUFF_SIZE, DEFAULT_VALUE) {
    f.open(filepath, ::binary_read_write);
    if (!f.is_open()) {
        f.open(filepath, ::binary_create_read_write);
    }
    fill_buffer();
}

file_tape::~file_tape() {
    flush();
}
namespace {
void sleep_if(std::int64_t delay) {
    if (delay > 0) {
        std::this_thread::sleep_for(mc(delay));
    }
}
}  // namespace

std::int32_t file_tape::get() const {
    sleep_if(delays.get_delay);
    return buff[index_in_buffer];
}

void file_tape::put(std::int32_t x) {
    sleep_if(delays.put_delay);
    buff[index_in_buffer] = x;
}

void file_tape::left() {
    sleep_if(delays.left_delay);
    if (index_in_buffer == 0 && buffers_from_start == 0) {
        throw std::runtime_error("signle direction tape error: reached left border");
    } else if (index_in_buffer > 0) {
        --index_in_buffer;
        return;
    }
    index_in_buffer = BUFF_SIZE - 1;

    handle_buffs(true);
}

void file_tape::right() {
    sleep_if(delays.right_delay);
    ++index_in_buffer;
    if (index_in_buffer < buff.size()) {
        return;
    }
    index_in_buffer = 0;

    handle_buffs(false);
}

void file_tape::reset() {
    sleep_if(delays.reset_delay);
    flush();
    is_prev_full = false;
    seek_buff(0);
    fill_buffer();
    buffers_from_start = 0;
    index_in_buffer = 0;
}

void file_tape::flush() {
    if (is_prev_full && is_prev_left) {
        seek_buff(buffers_from_start - 1);
        bin_util::write_vector(prev_buff, f);
    } else {
        seek_buff(buffers_from_start);
    }
    bin_util::write_vector(buff, f);
    if (is_prev_full && !is_prev_left) {
        bin_util::write_vector(prev_buff, f);
    }
}

void file_tape::seek_buff(std::size_t buffer_form_start) {
    f.seekg(buffer_form_start * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
}

void file_tape::handle_buffs(bool is_left) {
    auto offset = is_left ? -1 : 1;
    if (is_prev_full) {
        if (handle_prev(is_left)) {
            return;
        }
    }
    std::swap(buff, prev_buff);
    is_prev_full = true;
    is_prev_left = !is_left;

    seek_buff(buffers_from_start + offset);
    fill_buffer();

    buffers_from_start += offset;
}

bool file_tape::handle_prev(bool is_left) {
    auto offset = is_left ? -1 : 1;
    if (is_prev_left == is_left) {
        std::swap(buff, prev_buff);
        is_prev_left = !is_left;
        buffers_from_start += offset;
        return true;
    } else if (is_left || buffers_from_start > 0) {
        seek_buff(buffers_from_start - offset);
        bin_util::write_vector(prev_buff, f);
    }
    return false;
}

void file_tape::fill_buffer() {
    bin_util::read_vector(f, BUFF_SIZE, buff);
    if (f.eof()) {
        f.clear();
    }
}
