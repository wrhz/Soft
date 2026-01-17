#include "utils.h"

namespace utils {
    std::wstring utf8_to_wstring(const std::string& utf8_str) {
        if (utf8_str.empty()) return L"";
        
        int size_needed = MultiByteToWideChar(
            CP_UTF8, 0, 
            utf8_str.c_str(), (int)utf8_str.size(), 
            NULL, 0);
        
        std::wstring wstr(size_needed, 0);
        MultiByteToWideChar(
            CP_UTF8, 0,
            utf8_str.c_str(), (int)utf8_str.size(),
            &wstr[0], size_needed);
        
        return wstr;
    }
}
