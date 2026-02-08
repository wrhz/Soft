#ifndef X11_DRIVER_H
#define X11_DRIVER_H

#include <string>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <tuple>
#include <cairo/cairo-ft.h>
#include <filesystem>
#include "element.h"
#include "soft/types.h"
#include "nlohmann/json.hpp"

namespace fs = std::filesystem;


namespace driver {
    class X11Driver {
    public:
        X11Driver();
        ~X11Driver();
        
        int init(soft::types::Soft main_soft);
        void run();
        void finish();
        
    private:
        nlohmann::json soft_config;
        nlohmann::json platform_config;
        soft::types::Soft main_soft;
        soft::types::Element root_element;
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