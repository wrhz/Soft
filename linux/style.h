#ifndef __STYLE_H__
#define __STYLE_H__

#include <string>
#include <tuple>
#include <map>

namespace style {
    struct StyleStruct
    {
        int x = 0;
        int y = 0;
    };

    class Style
    {
    public:
        static StyleStruct handleStyle(int width, int height, int font_size, int text_width, int text_height, std::map<std::string, std::string> style);
    };
}

#endif