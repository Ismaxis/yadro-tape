#include "delays.h"

#include <json/json.h>

#include <fstream>

namespace config {
tape_delays delays(const std::string& config_file) {
    std::fstream config(config_file);
    if (!config.is_open()) {
        throw std::runtime_error("Can't open config file: " + config_file);
    }

    Json::Value root;
    Json::CharReaderBuilder b;
    JSONCPP_STRING errs;

    if (!parseFromStream(b, config, &root, &errs)) {
        throw std::runtime_error(errs);
    }

    tape_delays d;
    d.get_delay = root.get("get_delay", 0).asInt64();
    d.put_delay = root.get("put_delay", 0).asInt64();
    d.left_delay = root.get("left_delay", 0).asInt64();
    d.right_delay = root.get("right_delay", 0).asInt64();
    d.reset_delay = root.get("reset_delay", 0).asInt64();
    return d;
}
}  // namespace config
