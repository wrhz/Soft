#ifndef STYLE_H
#define STYLE_H

#include "soft/types.h"
#include "yoga/Yoga.h"

namespace soft::style {
    class Style {
    public:
        static void handle_style(YGNodeRef parent_node, std::map<std::string, std::string> styles);
    };
}

#endif // STYLE_H