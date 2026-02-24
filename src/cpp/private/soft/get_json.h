#ifndef SOFT_GET_JSON_H
#define SOFT_GET_JSON_H

#include <fstream>
#include <filesystem>

#include "nlohmann/json.hpp"

namespace fs = std::filesystem;

namespace soft::get_json {
    nlohmann::json file_get_json(fs::path path);
}

#endif