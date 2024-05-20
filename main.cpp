#include <gflags/gflags.h>
#include <sort/tape_sort.h>
#include <tape/file/file_tape.h>
#include <tape/tape.h>
#include <util/bin_util.h>
#include <util/file_util.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "config/delays.h"

DEFINE_string(input, "", "Path to input file");
DEFINE_string(output, "", "Path to output file");
DEFINE_string(config, "./config.json", "Path to config file");
DEFINE_bool(v, true, "Show info about tmp files and other");

auto is_empty_path = [](const char*, const std::string& path) {
    return !path.empty();
};

DEFINE_validator(input, is_empty_path);
DEFINE_validator(output, is_empty_path);

void error(const std::string& message) {
    std::cerr << "ERROR: " << message << std::endl;
}

std::size_t input_to_binary(std::string& input, std::filesystem::path& bin_in_filename) {
    std::fstream in(input, std::fstream::in);
    if (!in.is_open()) {
        throw std::runtime_error("Can't open input file: " + input);
    }

    std::fstream bin_in(bin_in_filename, std::fstream::binary | std::fstream::out | std::fstream::trunc);
    if (!bin_in.is_open()) {
        throw std::runtime_error(std::string("Can't create tmp input file: ") + bin_in_filename.generic_string());
    }
    std::size_t n = bin_util::to_binary(in, bin_in);
    if (FLAGS_v) {
        std::cout << "Found " << n << " elements" << std::endl;
    }
    return n;
}

void binary_to_output(std::string& ouptut, std::filesystem::path& bin_out_filename, size_t n) {
    {
        std::fstream out(ouptut, std::fstream::out | std::fstream::trunc);
        if (!out.is_open()) {
            throw std::runtime_error("Can't open output file: " + ouptut);
        }

        std::fstream bin_out(bin_out_filename, std::fstream::binary | std::fstream::in);
        if (!bin_out.is_open()) {
            throw std::runtime_error("Can't open tmp bin output file: " + bin_out_filename.generic_string());
        }

        bin_util::from_binary(bin_out, out, n);
    }
}

void tape_sort_binary(std::filesystem::path& bin_in_filename, std::size_t n, std::filesystem::path& bin_out_filename, tape_delays delays) {
    if (FLAGS_v) {
        std::cout << "Temp files:" << std::endl
                  << "  input: " << bin_in_filename << std::endl
                  << "  output: " << bin_out_filename << std::endl;
    }
    file_tape in_tape(bin_in_filename, delays);
    file_tape out_tape(bin_out_filename, delays);

    tape_sort(in_tape, n, out_tape, [&] {
        auto tmp = file_util::new_tmp_file();
        if (FLAGS_v) {
            std::cout << "  sorting tape: " << tmp << std::endl;
        }
        return std::make_unique<file_tape>(tmp, delays);
    });
}

int main(int argc, char** argv) {
    gflags::SetUsageMessage(
        "--input <input-file> --output <output-file> --config <config-file> [-v]. Or enter --help to see help page");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc > 1) {
        std::stringstream ss;
        for (size_t i = 1; i < argc; ++i) {
            ss << "'" << argv[i] << "' ";
        }
        error("Unknown argument(s): " + ss.str());
        return -1;
    }

    try {
        tape_delays d = config::delays(FLAGS_config);
        if (FLAGS_v) {
            std::cout << "Config: " << std::endl
                      << "  get_delay: " << d.get_delay << std::endl
                      << "  put_delay: " << d.put_delay << std::endl
                      << "  left_delay: " << d.left_delay << std::endl
                      << "  right_delay: " << d.right_delay << std::endl
                      << "  reset_delay: " << d.reset_delay << std::endl;
        }
        std::string input = FLAGS_input;
        std::string ouptut = FLAGS_output;

        auto bin_in_filename = file_util::new_tmp_file();
        auto bin_out_filename = file_util::new_tmp_file();

        std::size_t n = input_to_binary(input, bin_in_filename);

        std::cout << "Sorting..." << std::endl;

        tape_sort_binary(bin_in_filename, n, bin_out_filename, d);

        std::cout << "Sorted" << std::endl;

        binary_to_output(ouptut, bin_out_filename, n);
    } catch (const std::runtime_error& e) {
        error(e.what());
        return -1;
    }
}
