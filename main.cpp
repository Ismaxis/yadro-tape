#include <gflags/gflags.h>
#include <sort/tape_sort.h>
#include <tape/file/file_tape.h>
#include <util/bin_util.h>
#include <util/file_util.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>

DEFINE_string(input, "", "Path to input file");
DEFINE_string(output, "", "Path to output file");
DEFINE_bool(v, false, "Show info about tmp files and other");

static bool is_empty_path(const char*, const std::string& path) {
    return !path.empty();
}

DEFINE_validator(input, is_empty_path);
DEFINE_validator(output, is_empty_path);

void error(const std::string& message) {
    std::cerr << "ERROR: " << message << std::endl;
}

int main(int argc, char** argv) {
    gflags::SetUsageMessage(
        "--input <input-file> --output <output-file> [-v]. Or enter --help to see help page");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc > 1) {
        std::stringstream ss;
        for (size_t i = 1; i < argc; ++i) {
            ss << "'" << argv[i] << "' ";
        }
        error("Unknown argument(s): " + ss.str());
        return -1;
    }

    std::string input = FLAGS_input;
    std::string ouptut = FLAGS_output;

    auto bin_in_filename = file_util::new_tmp_file();
    auto bin_out_filename = file_util::new_tmp_file();

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
        if (FLAGS_v) {
            std::cout << "Found " << n << " elements" << std::endl;
        }
    }

    std::cout << "Sorting..." << std::endl;

    {
        if (FLAGS_v) {
            std::cout << "Temp files:" << std::endl
                      << "  input: " << bin_in_filename << std::endl
                      << "  output: " << bin_out_filename << std::endl;
        }
        file_tape in_tape(bin_in_filename);
        file_tape out_tape(bin_out_filename);

        tape_sort(in_tape, n, out_tape, [] {
            auto tmp = file_util::new_tmp_file();
            if (FLAGS_v) {
                std::cout << "  sorting tape: " << tmp << std::endl;
            }
            return std::make_unique<file_tape>(tmp);
        });
    }

    std::cout << "Sorted" << std::endl;

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

        bin_util::from_binary(bin_out, out, n);
    }
}
