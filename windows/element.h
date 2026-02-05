#ifndef ELEMENT_H
#define ELEMENT_H

#include <Windows.h>
#include <map>

#include "style.h"
#include "utils.h"
#include "soft/types.h"

namespace element {
    class Element {
    public:
        static void draw(soft::types::Element root_element, std::string font_family, HDC hdc, RECT rect);
    };
}

#endif // ELEMENT_H