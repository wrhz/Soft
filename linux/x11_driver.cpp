#include "x11_driver.h"
#include "element.h"
#include "freetype/freetype.h"
#include "nlohmann/json.hpp"
#include <X11/X.h>
#include <X11/Xutil.h>
#include <pybind11/pytypes.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <sys/auxv.h>

namespace driver {
    X11Driver::X11Driver() 
    : main_soft(),
      root_element(),
      size(),
      window(0),
      display(nullptr),
      elementObject(nullptr),
      title("Soft Linux"),
      default_font_family(),
      width(800),
      height(600),
      screen(0),
      screen_width(0),
      screen_height(0),
      font_size(24),
      library(),
      ft_face(),
      cairo_face(nullptr),
      exe_dir("")
    {}

    X11Driver::~X11Driver()
    {
        finish();
    }
    
    int X11Driver::init(soft::types::Soft main_soft)
    {
        exe_dir = fs::path((char*)getauxval(AT_EXECFN)).parent_path();

        display = XOpenDisplay(NULL);
        if (display == NULL)
        {
            std::cerr << "Cannot open X11 display" << std::endl;
            return 1;
        }

        screen = DefaultScreen(display);

        this->main_soft = main_soft;
        title = main_soft.title;
        if (title.length() > 30)
        {
            std::cerr << "The title length is not greater than 30" << std::endl;
            return 1;
        }

        size = main_soft.size;
        width = std::get<0>(size);
        height = std::get<1>(size);

        root_element = *main_soft.home;

        screen_width = DisplayWidth(display, screen);
        screen_height = DisplayHeight(display, screen);

        window = XCreateSimpleWindow(
            display,
            RootWindow(display, screen),
            (screen_width - width) / 2, (screen_height - height) / 2,
            width, height,
            1,
            BlackPixel(display, screen),
            WhitePixel(display, screen)
        );

        std::ifstream file((exe_dir / "config" / "font.json").string());
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string font_config = buffer.str(); 
        nlohmann::json font_json = nlohmann::json::parse(font_config);
        std::string font_family_name = main_soft.font_family;
        default_font_family = font_json["font_families"][font_family_name].get<std::string>();
        FT_Init_FreeType(&library);
        if (FT_New_Face(library, (exe_dir / "resources" / "fonts" / default_font_family).string().c_str(), 0, &ft_face) != 0) {
            std::cerr << "Error Cannot load font file: " 
                    << (exe_dir / "resources" / "fonts" / default_font_family).string() 
                    << std::endl;
            return 1;
        }
        cairo_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);
        font_size = font_json["font_size"].get<int>();

        XStoreName(display, window, title.length() == 0 ? "Soft Linux" : title.c_str());

        XSetWindowAttributes attrs;
        attrs.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
        XChangeWindowAttributes(display, window, CWEventMask, &attrs);
        
        XSizeHints hints;
        hints.flags = PPosition | PSize;
        hints.x = 100;
        hints.y = 100;
        hints.width = width;
        hints.height = height;
        XSetNormalHints(display, window, &hints);

        elementObject = std::make_shared<element::Element>(
            display, window,default_font_family, screen, root_element
        );

        cairo_surface_t *surface = cairo_xlib_surface_create(display, window,
                                                DefaultVisual(display, screen),
                                                width, height);
        cairo_t *cr = cairo_create(surface);

        cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_set_source_rgb(cr, 0, 0, 0);

        cairo_destroy(cr);
        cairo_surface_destroy(surface);

        return 0;
    }

    void X11Driver::run()
    {
        if (!display || !window) {
            std::cerr << "Window not initialized correctly" << std::endl;
            return;
        }

        XMapWindow(display, window);
        
        XMoveWindow(display, window, (screen_width - width) / 2, (screen_height - height) / 2);
        
        XFlush(display);
        
        bool running = true;
        XEvent event;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        while (running) {
            while (XPending(display) > 0) {
                XNextEvent(display, &event);
                
                switch (event.type) {
                    case ConfigureNotify:
                        width = event.xconfigure.width;
                        height = event.xconfigure.height;
                        break;
                    case Expose:
                        (*elementObject).draw(width,height, font_size, cairo_face);
                        break;
                    case KeyPress:
                    case ButtonPress:
                        running = false;
                        break;
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void X11Driver::finish()
    {
        if (window) {
            XDestroyWindow(display, window);
            window = 0;
        }
        
        if (display) {
            XCloseDisplay(display);
            display = nullptr;
        }

        cairo_font_face_destroy(cairo_face);
        FT_Done_Face(ft_face);
        FT_Done_FreeType(library);
    }
}