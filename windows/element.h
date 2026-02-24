#ifndef ELEMENT_H
#define ELEMENT_H

#include <Windows.h>
#include <map>

#include "soft/style.h"
#include "utils.h"
#include "soft/types.h"
#include "yoga/Yoga.h"

namespace element {
    class Element {
    public:
        static void set_style(YGNodeRef parent_node, soft::types::Element& element, HDC hdc);
        static void draw(soft::types::Element element, std::string font_family, HDC hdc, RECT rect, HFONT hFont);
    };
}

#endif // ELEMENT_H