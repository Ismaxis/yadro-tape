#pragma once
#include <json/json.h>

#include "../tape/tape.h"
namespace config {
tape_delays delays(const std::string& config_file);
}  // namespace config
