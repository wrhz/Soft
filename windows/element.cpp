#include "element.h"
#include "utils.h"
#include "vars.h"
#include "yoga/YGNodeLayout.h"
#include "yoga/YGNodeStyle.h"
#include <string>
#include <windef.h>
#include <wingdi.h>
#include <winuser.h>

namespace element {
    void Element::render_text(HDC hdc, soft::types::Element& element)
    {
        std::wstring text = utils::utf8_to_wstring(element.text);
        TextOutW(hdc, YGNodeLayoutGetLeft(element.node), YGNodeLayoutGetTop(element.node), text.c_str(), text.length());
    }

    void Element::render_image(HDC hdc, soft::types::Element& element)
    {
        if (element.image != nullptr)
        {
            Gdiplus::Graphics graphics(hdc);
            graphics.DrawImage(element.image, YGNodeLayoutGetLeft(element.node), YGNodeLayoutGetTop(element.node));
        }
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
        else if (tag == "image")
        {
            std::string src = element.src;

            std::string imagePath = (exe_dir / "resources" / "images" / src).string();

            element.image = new Gdiplus::Image(utils::utf8_to_wstring(imagePath).c_str());

            YGNodeStyleSetWidth(element.node, float(element.image->GetWidth()));
            YGNodeStyleSetHeight(element.node, float(element.image->GetHeight()));

            element.render = render_image;
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