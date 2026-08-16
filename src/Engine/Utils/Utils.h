#ifndef FARFIELD_UTILS_H
#define FARFIELD_UTILS_H

#include <filesystem>
#include <glm/glm.hpp>

namespace fs = std::filesystem;

namespace glm {
    inline std::string operator+(const std::string &lhs, const vec3& v)
    {
        return lhs + std::to_string(static_cast<int>(v.x)) + ", " + std::to_string(static_cast<int>(v.y)) + ", " + std::to_string(static_cast<int>(v.z));
    }

    inline std::ostream& operator<<(std::ostream& os, const vec3& v)
    {
        return os << v.x << ", " << v.y << ", " << v.z;
    }
}

namespace Files
{
    static std::string getResourcesPath(const std::string& path)
    {
        return RESOURCES_PATH + path;
    }
}

#endif