#include "Utils.hpp"

namespace SW {
    std::string Utils::ReadFile(const std::string& filepath) {
        std::string result;

        std::ifstream stream(filepath, std::ios::in | std::ios::binary);

        if (!stream.is_open())
            SW_ERROR("Could not open file %s", filepath.c_str());

        stream.seekg(0, std::ios::end);
        result.resize(stream.tellg());

        stream.seekg(0, std::ios::beg);
        stream.read(result.data(), result.size());
        stream.close();

        return result;
    }
}
