#include "style.h"

UINT style::Style::handle_style(HDC, RECT rect, py::dict styles)
{
    UINT format_style = DT_SINGLELINE;

    for (auto item : styles)
    {
        std::string key = item.first.cast<std::string>();
        std::string value = item.second.cast<std::string>();
        if (key == "horizontal_align")
        {
            if (value == "right")
            {
                format_style |= DT_RIGHT;
            }
            else if (value == "center")
            {
                format_style |= DT_CENTER;
            }
            else
            {
                format_style |= DT_LEFT;
            }
        }
        else
        {
            if (key == "vertical_align")
            {
                if (value == "bottom")
                {
                    format_style |= DT_BOTTOM;
                }
                else if (value == "middle")
                {
                    format_style |= DT_VCENTER;
                }
                else
                {
                    format_style |= DT_TOP;
                }
            }
        }
    }

    return format_style;
}
