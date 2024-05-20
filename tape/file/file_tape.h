#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <vector>

#include "../tape.h"

class file_tape : public tape {
    constexpr static std::size_t BUFF_SIZE = 512;

   public:
    file_tape(std::filesystem::path filepath);

    virtual ~file_tape() override;

    int get() const override;
    void put(int x) override;
    void left() override;
    void right() override;
    void reset() override;

   private:
    void flush();
    void seek_buff(std::size_t buffer_form_start);
    void handle_buffs(bool is_left);
    bool handle_prev(bool is_left);
    void fill_buffer();

   private:
    std::fstream f;
    std::size_t index_in_buffer{};
    std::size_t buffers_from_start{};
    std::vector<std::int32_t> buff;
    std::vector<std::int32_t> prev_buff;
    bool is_prev_full{false};
    bool is_prev_left{false};
};
