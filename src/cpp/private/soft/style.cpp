#include "style.h"

namespace soft::style
{
    void Style::handle_style(YGNodeRef node, std::map<std::string, std::string> styles)
    {        
        std::string display = styles["display"];

        YGNodeStyleSetMargin(node, YGEdgeAll, 5);

        for (auto item : styles)
        {
            std::string key = item.first;

            if (key == "display")
            {
                continue;
            }

            std::string value = item.second;
            
            if (display == "flex")
            {
                if (key == "flex_direction")
                {
                    if (value == "column")
                    {
                        YGNodeStyleSetFlexDirection(node, YGFlexDirectionColumn);
                    }
                    else
                    {
                        YGNodeStyleSetFlexDirection(node, YGFlexDirectionRow);
                    }
                }
                else if (key == "flex_wrap")
                {
                    if (value == "wrap")
                    {
                        YGNodeStyleSetFlexWrap(node, YGWrapWrap);
                    }
                    else if (value == "wrap-reverse")
                    {
                        YGNodeStyleSetFlexWrap(node, YGWrapWrapReverse);
                    }
                    else
                    {
                        YGNodeStyleSetFlexWrap(node, YGWrapNoWrap);
                    }
                }
                else if (key == "flex_grow")
                {
                    YGNodeStyleSetFlexGrow(node, stof(value));
                }
                else if (key == "justify_content")
                {
                    if (value == "flex-start")
                    {
                        YGNodeStyleSetJustifyContent(node, YGJustifyFlexStart);
                    }
                    else if (value == "flex-end")
                    {
                        YGNodeStyleSetJustifyContent(node, YGJustifyFlexEnd);
                    }
                    else if (value == "center")
                    {
                        YGNodeStyleSetJustifyContent(node, YGJustifyCenter);
                    }
                    else if (value == "space-between")
                    {
                        YGNodeStyleSetJustifyContent(node, YGJustifySpaceBetween);
                    }
                    else if (value == "space-around")
                    {
                        YGNodeStyleSetJustifyContent(node, YGJustifySpaceAround);
                    }
                    else
                    {
                        YGNodeStyleSetJustifyContent(node, YGJustifySpaceEvenly);
                    }
                }
                else if (key == "align_items")
                {
                    if (value == "flex-start")
                    {
                        YGNodeStyleSetAlignItems(node, YGAlignFlexStart);
                    }
                    else if (value == "flex-end")
                    {
                        YGNodeStyleSetAlignItems(node, YGAlignFlexEnd);
                    }
                    else if (value == "center")
                    {
                        YGNodeStyleSetAlignItems(node, YGAlignCenter);
                    }
                    else if (value == "stretch")
                    {
                        YGNodeStyleSetAlignItems(node, YGAlignStretch);
                    }
                    else
                    {
                        YGNodeStyleSetAlignItems(node, YGAlignBaseline);
                    }
                }
            }
            else if (key == "margin")
            {
                YGNodeStyleSetMargin(node, YGEdgeAll, stof(value));
            }
            else if (key == "margin_top")
            {
                YGNodeStyleSetMargin(node, YGEdgeTop, stof(value));
            }
            else if (key == "margin_right")
            {
                YGNodeStyleSetMargin(node, YGEdgeRight, stof(value));
            }
            else if (key == "margin_bottom")
            {
                YGNodeStyleSetMargin(node, YGEdgeBottom, stof(value));
            }
            else if (key == "margin_left")
            {
                YGNodeStyleSetMargin(node, YGEdgeLeft, stof(value));
            }
            else if (key == "padding")
            {
                YGNodeStyleSetPadding(node, YGEdgeAll, stof(value));
            }
            else if (key == "padding_top")
            {
                YGNodeStyleSetPadding(node, YGEdgeTop, stof(value));
            }
            else if (key == "padding_right")
            {
                YGNodeStyleSetPadding(node, YGEdgeRight, stof(value));
            }
            else if (key == "padding_bottom")
            {
                YGNodeStyleSetPadding(node, YGEdgeBottom, stof(value));
            }
            else if (key == "padding_left")
            {
                YGNodeStyleSetPadding(node, YGEdgeLeft, stof(value));
            }
        }
    }
}