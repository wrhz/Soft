#ifndef ELEMENT_H
#define ELEMENT_H

#include <Windows.h>
#include "style.h"
#include "utils.h"
#include "soft/types.h"
#include <map>

namespace element {
    class Element {
    public:
        static void draw(soft::types::ElementStruct root_element, HDC hdc, RECT rect);
    };
}

#endif // ELEMENT_H