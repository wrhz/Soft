#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <cairo/cairo-ft.h>
#include <list>

#include "style.h"
#include "soft/types.h"

namespace element
{
    class Element
    {
    public:
        Element();
        Element(Display* display, Window window, std::string font_family, int screen, soft::types::ElementStruct root_element);
        void draw(int width, int height, int font_size, cairo_font_face_t *cairo_face);

    private:
        Display* display;
        Window window;
        std::string font_family;
        int screen;
        soft::types::ElementStruct root_element;

        style::StyleStruct returnStyle(int width, int height, int font_size, std::string text, std::map<std::string, std::string> style_object, cairo_t* cr);
    };
}

#endif