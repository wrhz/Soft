#include "element.h"
#include "soft/types.h"
#include "yoga/YGNodeLayout.h"
#include <cairo/cairo.h>
#include <string>

namespace element
{
    void Element::set_style(cairo_t *cr, YGNodeRef parent_node, soft::types::Element& element)
    {
        element.node = YGNodeNew();
        soft::style::Style::handle_style(element.node, element.style);
        std::string tag = element.tag;

        if (tag == "text")
        {
            cairo_text_extents_t extents;
            cairo_text_extents(cr, element.text.c_str(), &extents);

            float x = float(extents.width) - extents.x_bearing;
            float y = float(extents.height) - extents.y_bearing;

            YGNodeStyleSetWidth(element.node, x);
            YGNodeStyleSetHeight(element.node, y);
        }

        YGNodeInsertChild(parent_node, element.node, YGNodeGetChildCount(parent_node));

        for (auto& child_element : element.children)
        {
            element::Element::set_style(cr, element.node, child_element);
        }
    }

    void Element::draw(cairo_t *cr, int width, int height, int font_size, cairo_font_face_t *cairo_face, Display* display, Window window, std::string font_family, int screen, soft::types::Element* _element)
    {
        cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_set_source_rgb(cr, 0, 0, 0);

        std::string tag = _element->tag;
        
        if (tag == "text")
        {
            std::string text = _element->text;
            
            cairo_move_to(cr, YGNodeLayoutGetLeft(_element->node), YGNodeLayoutGetTop(_element->node) + YGNodeLayoutGetHeight(_element->node));

            cairo_show_text(cr, text.c_str());

            cairo_stroke(cr);
        }

        for (auto& child_element : _element->children)
        {
            element::Element::draw(cr, width, height, font_size, cairo_face, display, window, font_family, screen, &child_element);
        }
    }
}