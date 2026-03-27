#include "element.h"
#include "utils.h"
#include "yoga/YGNodeLayout.h"
#include <string>

namespace element {
    void Element::render_text(HDC hdc, soft::types::Element& element)
    {
        std::wstring text = utils::utf8_to_wstring(element.text);
        TextOutW(hdc, YGNodeLayoutGetLeft(element.node), YGNodeLayoutGetTop(element.node), text.c_str(), text.length());
    }

    void Element::init_element(YGNodeRef parent_node, soft::types::Element& element, HDC hdc)
    {
        element.node = YGNodeNew();
        soft::style::Style::handle_style(element.node, element.style);
        std::string tag = element.tag;

        if (tag == "text")
        {
            std::wstring text = utils::utf8_to_wstring(element.text);
            SIZE size;
            GetTextExtentPoint32W(hdc, text.c_str(), text.length(), &size);

            YGNodeStyleSetWidth(element.node, float(size.cx));
            YGNodeStyleSetHeight(element.node, float(size.cy));

            element.render = render_text;
        }
        else {
            element.render = [](HDC hdc, soft::types::Element& element) {};
        }

        YGNodeInsertChild(parent_node, element.node, YGNodeGetChildCount(parent_node));

        for (int i = 0; i < element.children.size(); i++)
        {
            element::Element::init_element(element.node, *element.children[i], hdc);
        }
    }

    void Element::draw(soft::types::Element* element, std::string font_family, HDC hdc, RECT rect, HFONT hFont)
    {
        element->render(hdc, *element);

        for (int i = 0; i < element->children.size(); i++)
        {
            utils::console_log(".", "Drawing child element");
            soft::types::Element* child_element = element->children[i];
            Element::draw(child_element, font_family, hdc, rect, hFont);
        }
    }
}