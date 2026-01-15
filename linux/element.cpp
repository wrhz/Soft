#include "element.h"
#include "style.h"
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <string>
// #include <tuple>

namespace element
{
    Element::Element(Display* display, Window window, std::list<std::string> font_familys, int screen, py::object root_element)
    {
        this->display = display;
        this->window = window;
        this->font_familys = font_familys;
        this->screen = screen;
        this->root_element = root_element;
    }

    void Element::draw(int width, int height, int font_size)
    {
        cairo_surface_t *surface = cairo_xlib_surface_create(display, window,
                                                DefaultVisual(display, screen),
                                                width, height);
        cairo_t *cr = cairo_create(surface);

        cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_set_source_rgb(cr, 0, 0, 0);

        for (std::string font_family : font_familys) {
            cairo_select_font_face(cr, font_family.c_str(),
                                    CAIRO_FONT_SLANT_NORMAL,
                                    CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, font_size);
            
            cairo_text_extents_t extents;
            cairo_text_extents(cr, "测试", &extents);
            
            if (extents.width > 0) {
                break;
            }
        }

        std::string tag = root_element["tag"].cast<std::string>();
        py::object style_object = root_element["style"].attr("style");
        style::StyleStruct style;
        if (tag == "text")
        {
            std::string text = root_element["text"].cast<std::string>();

            style = returnStyle(width, height, font_size, text, style_object, cr);
            
            cairo_move_to(cr, style.x, style.y);
            cairo_show_text(cr, text.c_str());
        }

        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }

    style::StyleStruct Element::returnStyle(int width, int height, int font_size, std::string text, py::object style_object, cairo_t* cr)
    {
        style::StyleStruct style;
        if (!style_object.is_none()) {
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