#include "element.h"
#include "soft/types.h"
#include "style.h"
#include <string>

namespace element
{
    Element::Element(Display* display, Window window, std::string font_family, int screen, soft::types::ElementStruct root_element)
    {
        this->display = display;
        this->window = window;
        this->font_family = font_family;
        this->screen = screen;
        this->root_element = root_element;
    }

    void Element::draw(int width, int height, int font_size, cairo_font_face_t *cairo_face)
    {
        cairo_surface_t *surface = cairo_xlib_surface_create(display, window,
                                                DefaultVisual(display, screen),
                                                width, height);
        cairo_t *cr = cairo_create(surface);

        cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_set_source_rgb(cr, 0, 0, 0);

        cairo_set_font_size(cr, font_size);

        cairo_set_font_face(cr, cairo_face);

        std::string tag = root_element.tag;
        std::map<std::string, std::string> style_object = root_element.style;
        style::StyleStruct style;
        if (tag == "text")
        {
            std::string text = root_element.text;

            style = returnStyle(width, height, font_size, text, style_object, cr);
            
            cairo_move_to(cr, style.x, style.y);
            cairo_show_text(cr, text.c_str());
        }

        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }

    style::StyleStruct Element::returnStyle(int width, int height, int font_size, std::string text, std::map<std::string, std::string> style_object, cairo_t* cr)
    {
        style::StyleStruct style;
        if (!style_object.empty()) {
            cairo_text_extents_t text_extents;
            cairo_text_extents(cr, text.c_str(), &text_extents);
            style = style::Style::handleStyle(width, height, font_size, text_extents.width, text_extents.height, style_object);
        }
        else
        {
            style.x = font_size;
        }

        return style;
    }
}