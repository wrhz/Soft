#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <wingdi.h>
#include <shlwapi.h>
#include <filesystem>
#include <fstream>

#include "pybind11/embed.h"
#include "nlohmann/json.hpp"

#include "element.h"
#include "utils.h"
#include "soft/types.h"

namespace py = pybind11;
namespace fs = std::filesystem;

HWND hwnd;
HFONT hFont;
soft::types::Soft main_soft;
soft::types::Element root_element;
std::string default_font_family = "Arial";
fs::path exe_dir;

std::string GetExeParentDirString() {
    wchar_t exePath[MAX_PATH] = { 0 };
    
    DWORD length = GetModuleFileNameW(NULL, exePath, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        return "";
    }

    if (!PathRemoveFileSpecW(exePath)) {
        return "";
    }

    return utils::wstring_to_utf8(exePath);
}

void setup_console_for_python()
{
    AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::ios::sync_with_stdio();
}

void init()
{
    setup_console_for_python();

    py::module_ sys = py::module_::import("sys");

    fs::path exe_dir = GetExeParentDirString();
    
    fs::path lib_path = exe_dir / "lib";
    sys.attr("path").attr("append")(lib_path.string());

    fs::path src_path = exe_dir / "src";
    sys.attr("path").attr("append")(src_path.string());

    fs::path packages_path = exe_dir / "packages";
    sys.attr("path").attr("append")(packages_path.string());

    auto io_module = py::module_::import("io");
    
    auto py_stdout = io_module.attr("open")(
        "CONOUT$", "w", -1, "utf-8", "replace", "\n", true, true
    );
    sys.attr("stdout") = py_stdout;
    sys.attr("stderr") = io_module.attr("open")(
        "CONOUT$", "w", -1, "utf-8", "replace", "\n", true, true
    );
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_SIZE:
        {
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);

            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            int oldBkMode = SetBkMode(hdc, TRANSPARENT);

            element::Element::draw(root_element, default_font_family, hdc, rect);
            
            SelectObject(hdc, hOldFont);
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            FreeConsole();
            return 0;
        }
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    exe_dir = GetExeParentDirString();

    try
    {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

        atexit([]() {
            if (Py_IsInitialized()) {
                Py_Finalize();
            }
        });

        SetDllDirectoryA((exe_dir / "python").string().c_str());

        py::scoped_interpreter guard{};

        init();

        py::module_ main = py::module_::import("lib.main");
        py::object main_soft_object = main.attr("main")();

        main_soft = soft::types::Soft(main_soft_object);
        root_element = *main_soft.home;

        if (main_soft.title.length() > 30)
        {
            return 1;
        }

        std::string font_family_name = main_soft.font_family;

        std::ifstream file(exe_dir / "config" / "font.json");
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string font_config = buffer.str(); 
        nlohmann::json font_json = nlohmann::json::parse(font_config);

        default_font_family = font_json["font_families"][font_family_name].get<std::string>();
        int font_size = font_json["font_size"].get<int>();

        AddFontResourceEx((exe_dir / "resources" / "fonts" / default_font_family).wstring().c_str(), FR_PRIVATE, NULL);

        const wchar_t* className = L"SoftWindowsClass";
        
        WNDCLASSW wc = {0};

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = className;
        wc.hCursor = LoadCursorW(NULL, L"IDC_ARROW");
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        
        if (RegisterClassW(&wc) == 0)
        {
            MessageBoxW(NULL, L"Failed to register window class", L"Error", MB_ICONERROR);
            return 1;
        }
        
        hwnd = CreateWindowW(
            className,
            L"Soft Windows",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            640, 480,
            NULL, NULL, hInstance, NULL
        );

        hFont = CreateFontW(
            -font_size,
            0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            utils::utf8_to_wstring(font_family_name).c_str()
        );

        SendMessageW(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);

        if (hwnd == NULL)
        {
            OutputDebugString(TEXT("Failed to create window\n"));
            return 1;
        }

        std::wstring title = utils::utf8_to_wstring(main_soft.title);

        if (!title.empty())
        {
            wchar_t title_buffer[31];
            wcscpy_s(title_buffer, title.c_str());
            SetWindowTextW(hwnd, title_buffer);
        }

        py::tuple size = main_soft_object.attr("size").cast<py::tuple>();
        int width = size[0].cast<int>();
        int height = size[1].cast<int>();

        RECT rcWorkArea;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
        int screen_width = rcWorkArea.right - rcWorkArea.left;
        int screen_height = rcWorkArea.bottom - rcWorkArea.top;

        SetWindowPos(hwnd, NULL, (screen_width - width) / 2, (screen_height - height) / 2, width, height, SWP_NOZORDER);

        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        DeleteObject(hFont);
        RemoveFontResourceEx((exe_dir / "resources" / "fonts" / default_font_family).wstring().c_str(), FR_PRIVATE, NULL);
    }
    catch (const std::exception& e)
    {
        utils::console_log(exe_dir, e.what());
    }
    
    return 0;
}
