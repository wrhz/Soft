#ifndef X11_DRIVER_H
#define X11_DRIVER_H

#include <string>
#include <X11/Xlib.h>
#include <tuple>
#include <cairo/cairo-ft.h>
#include <filesystem>
#include "element.h"
#include "soft/types.h"

namespace fs = std::filesystem;


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
        std::string default_font_family;
        int width;
        int height;
        int screen;
        int screen_width;
        int screen_height;
        int font_size;
        FT_Library library;
        FT_Face ft_face;
        cairo_font_face_t *cairo_face;
        fs::path exe_dir;
    };
}

#endif // X11_DRIVER_H