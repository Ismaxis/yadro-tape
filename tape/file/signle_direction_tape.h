#pragma once

#include <bin_util.h>

#include <cstdint>
#include <fstream>
#include <vector>

#include "../i_tape.h"

namespace file {
class signle_direction_tape : public i_tape {
    constexpr static std::size_t BUFF_SIZE = 512;

   public:
    signle_direction_tape(const std::string& filename)
        : f(filename, std::ios::in | std::ios::out | std::ios::binary),
          buff(BUFF_SIZE),
          prev_buff(BUFF_SIZE) {
        if (!f) {
            f.open(filename, std::fstream::binary | std::fstream::trunc | std::fstream::out);
            f.close();
            f.open(filename, std::fstream::binary | std::fstream::in | std::fstream::out);
        }
        fill_buffer();
    }
    virtual ~signle_direction_tape() override {
        if (is_prev_full && is_prev_left) {
            f.seekg((buffers_from_start - 1) * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
            bin_util::write_vector(prev_buff, f);
        } else {
            f.seekg(buffers_from_start * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
        }
        bin_util::write_vector(buff, f);
        if (is_prev_full && !is_prev_left) {
            bin_util::write_vector(prev_buff, f);
        }
    }

    int get() const override {
        return buff[index_in_buffer];
    }
    void put(int x) override {
        buff[index_in_buffer] = x;
    }

   protected:
    void left_impl() override {
        if (index_in_buffer == 0 && buffers_from_start == 0) {
            throw std::runtime_error("signle direction tape error: reached left border");
        } else if (index_in_buffer > 0) {
            --index_in_buffer;
            return;
        }
        index_in_buffer = BUFF_SIZE - 1;

        if (is_prev_full) {
            if (handle_prev(true)) {
                return;
            }
            // if (is_prev_left) {
            //     std::swap(buff, prev_buff);
            //     is_prev_left = false;
            //     --buffers_from_start;
            //     return;
            // } else {
            //     f.seekg((buffers_from_start + 1) * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
            //     bin_util::write_vector(prev_buff, f);
            // }
        }
        std::swap(buff, prev_buff);
        is_prev_full = true;
        is_prev_left = false;

        f.seekg((buffers_from_start - 1) * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
        fill_buffer();

        --buffers_from_start;
    }
    void right_impl() override {
        ++index_in_buffer;
        if (index_in_buffer < buff.size()) {
            return;
        }
        index_in_buffer = 0;

        if (is_prev_full) {
            if (handle_prev(false)) {
                return;
            }
            // if (!is_prev_left) {
            //     std::swap(buff, prev_buff);
            //     is_prev_left = true;
            //     ++buffers_from_start;
            //     return;
            // } else if (buffers_from_start > 0) {
            //     f.seekg((buffers_from_start - 1) * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
            //     bin_util::write_vector(prev_buff, f);
            // }
        }
        std::swap(buff, prev_buff);
        is_prev_full = true;
        is_prev_left = true;

        f.seekg((buffers_from_start + 1) * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
        fill_buffer();

        ++buffers_from_start;
    }

   private:
    bool handle_prev(bool is_left) {
        auto offset = is_left ? -1 : 1;
        if (is_prev_left == is_left) {
            std::swap(buff, prev_buff);
            is_prev_left = !is_left;
            buffers_from_start += offset;
            return true;
        } else if (is_left || buffers_from_start > 0) {
            f.seekg((buffers_from_start - offset) * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
            bin_util::write_vector(prev_buff, f);
        }
        return false;
    }

    void fill_buffer() {
        bin_util::read_vector(f, BUFF_SIZE, buff);
        if (f.eof()) {
            f.clear();
        }
    }

   private:
    std::fstream f;
    std::size_t index_in_buffer{};
    std::size_t buffers_from_start{};
    std::vector<std::int32_t> buff;
    std::vector<std::int32_t> prev_buff;
    bool is_prev_full{false};
    bool is_prev_left{false};
};
}  // namespace file
