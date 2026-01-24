#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <shlwapi.h>
#include <pybind11/embed.h>
#include <filesystem>
#include "element.h"
#include "utils.h"
#include "soft/types.h"

namespace py = pybind11;
namespace fs = std::filesystem;

HWND hwnd;
soft::types::SoftStruct main_soft_struct;
soft::types::ElementStruct root_element_struct;

std::string WideStringToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, 
                                           wstr.c_str(), (int)wstr.size(),
                                           NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, 
                        wstr.c_str(), (int)wstr.size(),
                        &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::string GetExeParentDirString() {
    wchar_t exePath[MAX_PATH] = { 0 };
    
    DWORD length = GetModuleFileNameW(NULL, exePath, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        return "";
    }

    if (!PathRemoveFileSpecW(exePath)) {
        return "";
    }

    return WideStringToUtf8(exePath);
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
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            element::Element::draw(root_element_struct, hdc, rect);
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
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

    atexit([]() {
        if (Py_IsInitialized()) {
            Py_Finalize();
        }
    });

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

    if (hwnd == NULL)
    {
        OutputDebugString(TEXT("Failed to create window\n"));
        return 1;
    }

    SetDllDirectoryA(".\\python-3.12.9-embed-amd64");

    py::scoped_interpreter guard{};

    init();

    py::module_ main = py::module_::import("lib.main");
    py::object main_soft = main.attr("main")();
    std::wstring title = utils::utf8_to_wstring(main_soft_struct.title);

    soft::types::init_soft_struct(main_soft, main_soft_struct);
    root_element_struct = *main_soft_struct.home;

    if (main_soft_struct.title.length() > 30)
    {
        return 1;
    }
    else if (!title.empty())
    {
        wchar_t title_buffer[31];
        wcscpy_s(title_buffer, title.c_str());
        SetWindowTextW(hwnd, title_buffer);
    }

    py::tuple size = main_soft.attr("size").cast<py::tuple>();
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
    
    return 0;
}
