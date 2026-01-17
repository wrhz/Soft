#ifndef X11_DRIVER_H
#define X11_DRIVER_H

#include <string>
#include <X11/Xlib.h>
#include <tuple>
#include "element.h"
#include "soft/types.h"

namespace driver {
    class X11Driver {
    public:
        X11Driver();
        ~X11Driver();
        
        int init(soft::types::SoftStruct main_soft_struct);
        void run();
        void finish();
        
    private:
        soft::types::SoftStruct main_soft_struct;
        soft::types::ElementStruct root_element_struct;
        std::tuple<int, int> size;
        Window window;
        Display* display;
        std::shared_ptr<element::Element> elementObject;
        std::string title;
        std::list<std::string> font_familys;
        int width;
        int height;
        int screen;
        int screen_width;
        int screen_height;
        int font_size;
    };
}

#endif // X11_DRIVER_H