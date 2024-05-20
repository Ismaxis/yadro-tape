#pragma once

#include <bin_util.h>

#include <cstdint>
#include <fstream>
#include <vector>

#include "../tape.h"

// TODO: .cpp
class file_tape : public tape {
    constexpr static std::size_t BUFF_SIZE = 512;

   public:
    file_tape(const std::string& filename)
        : buff(BUFF_SIZE),
          prev_buff(BUFF_SIZE) {
        f.open(filename, std::fstream::binary | std::fstream::in | std::fstream::out);
        if (!f.is_open()) {
            f.open(filename, std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
        }
        fill_buffer();
    }

    virtual ~file_tape() override {
        flush();
    }

    int get() const override {
        return buff[index_in_buffer];
    }

    void put(int x) override {
        buff[index_in_buffer] = x;
    }

    void left() override {
        if (index_in_buffer == 0 && buffers_from_start == 0) {
            throw std::runtime_error("signle direction tape error: reached left border");
        } else if (index_in_buffer > 0) {
            --index_in_buffer;
            return;
        }
        index_in_buffer = BUFF_SIZE - 1;

        handle_buffs(true);
    }

    void right() override {
        ++index_in_buffer;
        if (index_in_buffer < buff.size()) {
            return;
        }
        index_in_buffer = 0;

        handle_buffs(false);
    }

    void reset() override {
        flush();
        is_prev_full = false;
        seek_buff(0);
        fill_buffer();
        buffers_from_start = 0;
        index_in_buffer = 0;
    }

   private:
    void flush() {
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

    void seek_buff(std::size_t buffer_form_start) {
        f.seekg(buffer_form_start * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
    }

    void handle_buffs(bool is_left) {
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

    bool handle_prev(bool is_left) {
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