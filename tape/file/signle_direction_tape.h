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
          buff(BUFF_SIZE) {
        if (!f) {
            f.open(filename, std::fstream::binary | std::fstream::trunc | std::fstream::out);
            f.close();
            f.open(filename, std::fstream::binary | std::fstream::in | std::fstream::out);
        }
        fill_buffer();
    }
    virtual ~signle_direction_tape() override {
        f.seekg(buffers_from_start * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
        bin_util::write_vector(buff, f);
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

        f.seekg(buffers_from_start * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
        bin_util::write_vector(buff, f);

        f.seekg((buffers_from_start - 1) * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
        fill_buffer();

        --buffers_from_start;
        index_in_buffer = BUFF_SIZE - 1;
    }
    void right_impl() override {
        ++index_in_buffer;
        if (index_in_buffer < buff.size()) {
            return;
        };
        f.seekg(buffers_from_start * BUFF_SIZE * sizeof(std::int32_t) - f.tellg(), f.cur);
        bin_util::write_vector(buff, f);

        fill_buffer();

        ++buffers_from_start;
        index_in_buffer = 0;
    }

   private:
    void fill_buffer() {
        bin_util::read_vector(f, BUFF_SIZE, buff);
        if (f.eof()) {
            f.clear();
        }
    }

   private:
    std::fstream f;
    std::vector<std::int32_t> buff;
    std::size_t index_in_buffer{};
    std::size_t buffers_from_start{};
};
}  // namespace file
