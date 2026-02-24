#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <cairo/cairo-ft.h>
#include <list>

#include "soft/types.h"
#include "soft/style.h"

namespace element
{
    class Element
    {
    public:
        Element();
        static void set_style(cairo_t *cr, YGNodeRef parent_node, soft::types::Element& element);
        static void draw(cairo_t *cr, int width, int height, int font_size, cairo_font_face_t *cairo_face, Display* display, Window window, std::string font_family, int screen, soft::types::Element* _element);
    };
}

#endif