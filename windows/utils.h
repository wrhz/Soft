#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace utils
{
    std::wstring utf8_to_wstring(const std::string& utf8_str);
    std::string wstring_to_utf8(const std::wstring& wstr);
    void console_log(const fs::path dir_path, const std::string log);
}

#endif // !UTILS_H