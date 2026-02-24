#include "element.h"
#include "utils.h"

namespace element {
    void Element::set_style(YGNodeRef parent_node, soft::types::Element& element, HDC hdc)
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
        }

        YGNodeInsertChild(parent_node, element.node, YGNodeGetChildCount(parent_node));

        for (auto& child_element : element.children)
        {
            element::Element::set_style(element.node, child_element, hdc);
        }
    }

    void Element::draw(soft::types::Element element, std::string font_family, HDC hdc, RECT rect, HFONT hFont)
    {
        std::string tag = element.tag;

        if (tag == "text")
        {
            std::wstring text = utils::utf8_to_wstring(element.text);

            TextOutW(hdc, int(YGNodeLayoutGetLeft(element.node)), int(YGNodeLayoutGetTop(element.node)), text.c_str(), text.length());
        }

        for (auto& child_element : element.children)
        {
            element::Element::draw(child_element, font_family, hdc, rect, hFont);
        }
    }
}