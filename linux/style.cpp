#include "style.h"

namespace style
{
    StyleStruct Style::handleStyle(int width, int height, int font_size, int text_width, int text_height, py::dict style)
    {
        StyleStruct styleStruct;
        styleStruct.y = font_size;
        for (auto item : style)
        {
            std::string key = item.first.cast<std::string>();
            std::string value = item.second.cast<std::string>();

            if (key == "horizontal_align")
            {
                if (value == "right")
                {
                    styleStruct.x = width - text_width - 5;
                }
                else if (value == "center")
                {
                    styleStruct.x = (width - text_width) / 2;
                }
                else
                {
                    styleStruct.x = font_size;
                }
            }
            else if (key == "vertical_align")
            {
                if (value == "bottom")
                {
                    styleStruct.y = height - text_height;
                }
                else if (value == "middle")
                {
                    styleStruct.y = (height - text_height) / 2;
                }
                else
                {
                    styleStruct.y = 0;
                }
            }
        }

        return styleStruct;
    }
}
