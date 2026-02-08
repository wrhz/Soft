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

    std::string wstring_to_utf8(const std::wstring& wstr) {
        if (wstr.empty()) return "";
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, 
                                            wstr.c_str(), (int)wstr.size(),
                                            NULL, 0, NULL, NULL);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, 
                            wstr.c_str(), (int)wstr.size(),
                            &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }

    void console_log(const fs::path dir_path, const std::string log)
    {
        if (!fs::exists(dir_path))
            fs::create_directories(dir_path);

        std::ofstream log_file(dir_path / "log.txt", std::ios_base::app);
        log_file << "Error: " << log << std::endl;
        log_file.close();
    }
}
