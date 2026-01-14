#include "style.h"

namespace style
{
    std::tuple<int, int> Style::handleStyle(int width, int height, int font_size, int text_width, int text_height, py::dict style)
    {
        int h = font_size, w = 0;
        for (auto item : style)
        {
            std::string key = item.first.cast<std::string>();
            std::string value = item.second.cast<std::string>();

            if (key == "horizontal_align")
            {
                if (value == "right")
                {
                    w = width - text_width - 5;
                }
                else if (value == "center")
                {
                    w = (width - text_width) / 2;
                }
                else
                {
                    w = font_size;
                }
            }
            else if (key == "vertical_align")
            {
                if (value == "bottom")
                {
                    h = height - text_height;
                }
                else if (value == "middle")
                {
                    h = (height - text_height) / 2;
                }
                else
                {
                    h = 0;
                }
            }
        }

        return std::make_tuple(w, h);
    }
}
