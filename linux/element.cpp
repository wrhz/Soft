#include "element.h"
#include "soft/types.h"
#include "yoga/YGNodeLayout.h"
#include "yoga/YGNodeStyle.h"
#include <cairo/cairo.h>
#include <string>

namespace element
{
    void Element::render_text(cairo_t *cr, soft::types::Element& element)
    {
        std::string text = element.text;
            
        cairo_move_to(cr, YGNodeLayoutGetLeft(element.node), YGNodeLayoutGetTop(element.node) + YGNodeLayoutGetHeight(element.node));

        cairo_show_text(cr, text.c_str());

        cairo_stroke(cr);
    }

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

            element.render = render_text;
        }
        else {
            element.render = [](cairo_t *cr, soft::types::Element& element) {};
        }

        YGNodeInsertChild(parent_node, element.node, YGNodeGetChildCount(parent_node));

        for (auto& child_element : element.children)
        {
            element::Element::set_style(cr, element.node, *child_element);
        }
    }

    void Element::draw(cairo_t *cr, int width, int height, int font_size, cairo_font_face_t *cairo_face, std::string font_family, soft::types::Element* _element)
    {
        cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_paint(cr); 

        cairo_set_source_rgb(cr, 0, 0, 0);

        _element->render(cr, *_element);

        for (auto& child_element : _element->children)
        {
            element::Element::draw(cr, width, height, font_size, cairo_face, font_family, child_element);
        }
    }
}