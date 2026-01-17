#ifndef ELEMENT_H
#define ELEMENT_H

#include <windows.h>
#include "style.h"
#include "soft/types.h"
#include <map>

namespace element {
    class Element {
    public:
        static void draw(soft::types::ElementStruct root_element, HDC hdc, RECT rect);
    };
}

#endif // ELEMENT_H