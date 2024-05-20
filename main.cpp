#include <sort/tape_sort.h>
#include <tape/file/file_tape.h>
#include <util/bin_util.h>
#include <util/file_util.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>

auto input = "D:/C++/yadro-tape/in.txt";
auto ouptut = "D:/C++/yadro-tape/out.txt";

int main() {
    auto bin_in_filename = file_util::new_tmp_file();
    auto bin_out_filename = file_util::new_tmp_file();
    std::cout << bin_in_filename << ' ' << bin_out_filename << '\n';
    std::size_t n;
    {
        std::fstream in(input, std::fstream::in);
        if (!in.is_open()) {
            std::cerr << "Can't open input file: " << input << std::endl;
            return -1;
        }

        std::fstream bin_in(bin_in_filename, std::fstream::binary | std::fstream::out | std::fstream::trunc);
        if (!bin_in.is_open()) {
            std::cerr << "Can't create tmp input file: " << bin_in_filename << std::endl;
            return -1;
        }
        n = bin_util::to_binary(in, bin_in);
        std::cout << n << '\n';
    }

    {
        file_tape in_tape(bin_in_filename);
        file_tape out_tape(bin_out_filename);

        tape_sort(in_tape, n, out_tape, [] {
            return std::make_unique<file_tape>(file_util::new_tmp_file());
        });

        std::cout << "Sorted\n";
    }

    {
        std::fstream out(ouptut, std::fstream::out | std::fstream::trunc);
        if (!out.is_open()) {
            std::cerr << "Can't open output file: " << ouptut << std::endl;
            return -1;
        }

        std::fstream bin_out(bin_out_filename, std::fstream::binary | std::fstream::in);
        if (!bin_out.is_open()) {
            std::cerr << "Can't open tmp bin output file: " << bin_out_filename << std::endl;
            return -1;
        }

        std::cout << bin_util::from_binary(bin_out, out, n) << '\n';
    }
}
