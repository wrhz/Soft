#ifndef STYLE_H
#define STYLE_H

#include <windows.h>
#include "soft/types.h"

namespace style {
    struct StyleStruct
    {
        UINT format_style = DT_SINGLELINE;
    };

    class Style {
    public:
        static void handle_style(HDC hdc, RECT rect, std::map<std::string, std::string> styles, StyleStruct& style_struct);
    };
}

#endif // STYLE_H