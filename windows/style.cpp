#include "style.h"

namespace style
{
    StyleStruct Style::handle_style(HDC hdc, RECT rect, py::dict styles)
    {
        StyleStruct style_struct;

        for (auto item : styles)
        {
            std::string key = item.first.cast<std::string>();
            std::string value = item.second.cast<std::string>();
            if (key == "horizontal_align")
            {
                if (value == "right")
                {
                    style_struct.format_style |= DT_RIGHT;
                }
                else if (value == "center")
                {
                    style_struct.format_style |= DT_CENTER;
                }
                else
                {
                    style_struct.format_style |= DT_LEFT;
                }
            }
            else
            {
                if (key == "vertical_align")
                {
                    if (value == "bottom")
                    {
                        style_struct.format_style |= DT_BOTTOM;
                    }
                    else if (value == "middle")
                    {
                        style_struct.format_style |= DT_VCENTER;
                    }
                    else
                    {
                        style_struct.format_style |= DT_TOP;
                    }
                }
            }
        }

        return style_struct;
    }
}