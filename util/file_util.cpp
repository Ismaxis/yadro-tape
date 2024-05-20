#include "file_util.h"

namespace file_util {
std::filesystem::path new_tmp_file() {
    return std::filesystem::temp_directory_path() / std::filesystem::path(std::tmpnam(nullptr)).filename();
}
}  // namespace file_util
