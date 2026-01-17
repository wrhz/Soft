#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>
#include <string>

namespace utils
{
    std::wstring utf8_to_wstring(const std::string& utf8_str);
}

#endif // !UTILS_H