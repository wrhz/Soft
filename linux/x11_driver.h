#ifndef X11_DRIVER_H
#define X11_DRIVER_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>
#include <string>
#include <X11/Xlib.h>
#include "element.h"

namespace py = pybind11;

namespace driver {
    class X11Driver {
    public:
        X11Driver();
        ~X11Driver();
        
        int init(py::object mainSoft);
        void run();
        void finish();
        
    private:
        py::object mainSoft;
        py::dict root_element;
        py::tuple size;
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