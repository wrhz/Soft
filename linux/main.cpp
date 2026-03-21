#include <X11/X.h>
#include <cairo/cairo-ft.h>
#include <cairo/cairo.h>
#include <pybind11/embed.h>
#include <filesystem>
#include <iostream>
#include <libgen.h>
#include <register_modules.h>
#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>
#include <sys/auxv.h>
#include <freetype/freetype.h>
#include <yoga/Yoga.h>

#include "soft/types.h"
#include "soft/get_json.h"
#include "element.h"
#include "types.h"
#include "yoga/YGConfig.h"

namespace py = pybind11;
namespace fs = std::filesystem;

FT_Library library;
FT_Face ft_face;

cairo_font_face_t *cairo_face;

nlohmann::json soft_config;
nlohmann::json platform_config;
nlohmann::json font_config;
fs::path exe_dir;
types::Font font;
YGNodeRef root_node;

std::string get_exe_parent_dir()
{
    char exe_path[PATH_MAX];
    char *parent_dir;

    ssize_t count = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (count == -1) {
        perror("readlink failed");
        return "";
    }
    exe_path[count] = '\0';

    char path_copy[PATH_MAX];
    strncpy(path_copy, exe_path, sizeof(path_copy));
    parent_dir = dirname(path_copy);

    char parent_dir_saved[PATH_MAX];
    strncpy(parent_dir_saved, parent_dir, sizeof(parent_dir_saved) - 1);
    parent_dir_saved[sizeof(parent_dir_saved) - 1] = '\0';

    return parent_dir_saved;
}

void init()
{
    py::module_ sys = py::module_::import("sys");

    fs::path exe_dir = get_exe_parent_dir();
    
    fs::path lib_path = exe_dir / "lib";
    sys.attr("path").attr("append")(lib_path.string());

    fs::path src_path = exe_dir / "src";
    sys.attr("path").attr("append")(src_path.string());

    fs::path packages_path = exe_dir / "packages";
    sys.attr("path").attr("append")(packages_path.string());

    initialize_python_module();
}

cairo_font_face_t *create_font_face(std::string default_font_family)
{
    FT_Init_FreeType(&library);
    if (FT_New_Face(library, (exe_dir / "resources" / "fonts" / default_font_family).string().c_str(), 0, &ft_face) != 0) {
        std::cerr << "Error Cannot load font file: " 
                << (exe_dir / "resources" / "fonts" / default_font_family).string() 
                << std::endl;
        return nullptr;
    }
    return cairo_ft_font_face_create_for_ft_face(ft_face, 0);
}

void sdl_texture_draw_with_cairo(soft::types::Soft main_soft, py::object page, SDL_Renderer *renderer, SDL_Texture *texture, int width, int height) {
    void *pixels;
    int pitch;

    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
        std::cerr << "SDL_LockTexture failed: " << SDL_GetError();
        return;
    }

    soft::types::Element* root_element = new soft::types::Element(page.attr("element"));

    cairo_surface_t *cs = cairo_image_surface_create_for_data(
        (unsigned char *)pixels,
        CAIRO_FORMAT_ARGB32,
        width, height,
        pitch
    );
    cairo_t *cr = cairo_create(cs);

    cairo_set_font_size(cr, font.font_size);

    cairo_set_font_face(cr, cairo_face);

    element::Element::set_style(cr, root_node, *root_element);

    YGNodeCalculateLayout(root_node, float(width), float(height), YGDirectionLTR);

    element::Element::draw(cr, width, height, font.font_size, cairo_face,font.default_font_family_name, root_element);
    
    delete root_element;

    cairo_destroy(cr);
    cairo_surface_destroy(cs);

    SDL_UnlockTexture(texture);
}

int main(int argc, char* argv[])
{
    try {
        py::scoped_interpreter guard{};

        init();

        exe_dir = fs::path((char*)getauxval(AT_EXECFN)).parent_path();

        soft_config = soft::get_json::file_get_json(exe_dir / "config" / "soft.json");
        platform_config = soft::get_json::file_get_json(exe_dir / "config" / "platform_config.json")["linux"];
        font_config = soft::get_json::file_get_json(exe_dir / "config" / "font.json");

        font.font_size = font_config["font_size"];

        root_node = YGNodeNew();

        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_Event event;
        int running = 1;

        SDL_Init(SDL_INIT_VIDEO);

        py::module_ main_module = py::module_::import("lib.main");
        py::object main_soft = main_module.attr("main")();

        soft::types::Soft main_soft_struct(main_soft);

        py::object page = main_soft_struct.home();
        std::string title = soft_config["title"];
        if (title.length() > 30)
        {
            std::cerr << "The title length is not greater than 30" << std::endl;
            return 1;
        }

        int width = platform_config["width"] == 0 ? 800 : platform_config["width"].get<int>();
        int height = platform_config["height"] == 0 ? 600 : platform_config["height"].get<int>();

        font.default_font_family_name = font_config["font_families"][main_soft_struct.default_font_family];

        cairo_face = create_font_face(font.default_font_family_name);
        if (cairo_face == nullptr) {
            std::cerr << "Error Cannot create font face" << std::endl;
            return 1;
        }

        window = SDL_CreateWindow(title.length() == 0 ? "Soft Application" : title.c_str(),
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width, height,
                              SDL_WINDOW_SHOWN);

        renderer = SDL_CreateRenderer(window, -1, 0);

        SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, width == 0 ? 800 : width, height == 0 ? 600 : height);

        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = 0;
                }
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            sdl_texture_draw_with_cairo(main_soft_struct, page, renderer, texture, width, height);

            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        YGNodeFree(root_node);
        destroy_python_module();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}