#include "element.h"

void element::Element::draw(soft::types::ElementStruct root_element, std::string font_family, HDC hdc, RECT rect)
{
    std::map<std::string, std::string> style_object = root_element.style;
    style::StyleStruct style;
    style::Style::handle_style(hdc, rect, style_object, style);
    std::string tag = root_element.tag;
    if (tag == "text")
    {
        std::wstring text = utils::utf8_to_wstring(root_element.text);
        DrawTextW(hdc, text.c_str(), -1, &rect, style.format_style);
    }
}
