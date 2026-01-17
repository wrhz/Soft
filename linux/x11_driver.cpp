#include "x11_driver.h"
#include "element.h"
#include <X11/Xutil.h>
#include <pybind11/pytypes.h>
#include <thread>
#include <iostream>

namespace driver {
    X11Driver::X11Driver() 
    : main_soft_struct(),
      root_element_struct(),
      size(),
      window(0),
      display(nullptr),
      elementObject(nullptr),
      title("Soft Linux"),
      font_familys(),
      width(800),
      height(600),
      screen(0),
      screen_width(0),
      screen_height(0),
      font_size(24)
    {}

    X11Driver::~X11Driver()
    {
        finish();
    }
    
    int X11Driver::init(soft::types::SoftStruct main_soft_struct)
    {
        display = XOpenDisplay(NULL);
        if (display == NULL)
        {
            std::cerr << "无法打开X11显示" << std::endl;
            return 1;
        }

        screen = DefaultScreen(display);

        this->main_soft_struct = main_soft_struct;
        title = main_soft_struct.title;
        if (title.length() > 30)
        {
            std::cerr << "The title length is not greater than 30" << std::endl;
            return 1;
        }

        size = main_soft_struct.size;
        width = std::get<0>(size);
        height = std::get<1>(size);

        root_element_struct = *main_soft_struct.home;

        for (std::string font_family : main_soft_struct.font_familys)
        {
            font_familys.push_back(font_family);
        }

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

        XStoreName(display, window, title.length() == 0 ? "Soft Linux" : title.c_str());

        XSetWindowAttributes attrs;
        attrs.event_mask = ExposureMask | KeyPressMask | ButtonPressMask;
        XChangeWindowAttributes(display, window, CWEventMask, &attrs);
        
        XSizeHints hints;
        hints.flags = PPosition | PSize;
        hints.x = 100;
        hints.y = 100;
        hints.width = width;
        hints.height = height;
        XSetNormalHints(display, window, &hints);

        elementObject = std::make_shared<element::Element>(
            display, window, font_familys, screen, root_element_struct
        );

        return 0;
    }

    void X11Driver::run()
    {
        if (!display || !window) {
            std::cerr << "窗口未正确初始化" << std::endl;
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
                    case Expose:
                        (*elementObject).draw(width, height, font_size);
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
    }
}