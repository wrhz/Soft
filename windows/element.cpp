#include "element.h"

void element::Element::draw(py::dict root_element, HDC hdc, RECT rect)
{
    py::object style_object = root_element["style"];
    UINT format_style;
    if (!style_object.is(py::none()))
    {
        format_style = style::Style::handle_style(hdc, rect, style_object.attr("style"));
    }
    else
    {
        format_style = DT_SINGLELINE;
    }
    std::string tag = root_element["tag"].cast<std::string>();
    if (tag == "text")
    {
        std::wstring text = root_element["text"].cast<std::wstring>();
        DrawTextW(hdc, text.c_str(), -1, &rect, format_style);
    }
}