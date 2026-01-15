#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <pybind11/embed.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <list>

#include "style.h"

namespace py = pybind11;

namespace element
{
    class Element
    {
    public:
        Element();
        Element(Display* display, Window window, std::list<std::string> font_familys, int screen, py::object root_element);
        void draw(int width, int height, int font_size);

    private:
        Display* display;
        Window window;
        std::list<std::string> font_familys;
        int screen;
        py::object root_element;

        style::StyleStruct returnStyle(int width, int height, int font_size, std::string text, py::object style_object, cairo_t* cr);
    };
}

#endif