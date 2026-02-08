#include "get_json.h"

namespace soft::get_json {
    nlohmann::json file_get_json(fs::path path)
    {
        std::ifstream file(path);
        nlohmann::json json;
        file >> json;
        return json;
    }
}
