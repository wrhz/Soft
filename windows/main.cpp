#define UNICODE
#define _UNICODE

#include <windows.h>
#include <pybind11/embed.h>
#include <filesystem>
#include <string>
#include <cstdio>
#include <io.h>
#include <fcntl.h>

namespace py = pybind11;
namespace fs = std::filesystem;

HWND hWnd;
py::object rootElement;

void setupConsoleForPython()
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
    setupConsoleForPython();

    fs::path current_path = fs::current_path();
    
    fs::path project_root = current_path.parent_path().parent_path();
    
    py::module_ sys = py::module_::import("sys");
    
    sys.attr("path").attr("append")(project_root.string());
    
    fs::path lib_path = project_root / "lib";
    sys.attr("path").attr("append")(lib_path.string());

    fs::path src_path = project_root / "src";
    sys.attr("path").attr("append")(src_path.string());

    auto io_module = py::module_::import("io");
    
    auto py_stdout = io_module.attr("open")(
        "CONOUT$", "w", -1, "utf-8", "replace", "\n", true, true
    );
    sys.attr("stdout") = py_stdout;
    sys.attr("stderr") = io_module.attr("open")(
        "CONOUT$", "w", -1, "utf-8", "replace", "\n", true, true
    );
}

UINT handleFormatStyle(HDC, RECT rect, py::dict styles)
{
    UINT formatStyle = DT_SINGLELINE;

    for (auto item : styles)
    {
        std::string key = item.first.cast<std::string>();
        py::object value = item.second.cast<py::object>();
        if (key == "vertical_align")
        {
            std::string align = value.cast<std::string>();
            if (align == "top")
            {
                formatStyle |= DT_TOP;
            }
            else if (align == "bottom")
            {
                formatStyle |= DT_BOTTOM;
            }
            else if (align == "middle")
            {
                formatStyle |= DT_VCENTER;
            }
        }
        else if (key == "horizontal_align")
        {
            std::string align = value.cast<std::string>();
            if (align == "left")
            {
                formatStyle |= DT_LEFT;
            }
            else if (align == "right")
            {
                formatStyle |= DT_RIGHT;
            }
            else if (align == "center")
            {
                formatStyle |= DT_CENTER;
            }
        }
    }

    return formatStyle;
}

void draw(HDC hdc, RECT rect)
{
    py::object styleObject = rootElement["format_style"];
    UINT formatStyle;
    if (styleObject != py::none())
    {
        formatStyle = handleFormatStyle(hdc, rect, styleObject.attr("style"));
    }
    else
    {
        formatStyle = DT_SINGLELINE;
    }
    std::string tag = rootElement["tag"].cast<std::string>();
    if (tag == "text")
    {
        std::wstring text = rootElement["text"].cast<std::wstring>();
        DrawTextW(hdc, text.c_str(), -1, &rect, formatStyle);
    }
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
            draw(hdc, rect);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            FreeConsole();
            return 0;
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
    
    hWnd = CreateWindowW(
        className,
        L"Soft Windows",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        640, 480,
        NULL, NULL, hInstance, NULL
    );

    if (hWnd == NULL)
    {
        OutputDebugString(TEXT("Failed to create window\n"));
        return 1;
    }

    py::scoped_interpreter guard{};

    init();

    py::module_ main = py::module_::import("lib.main");
    py::object mainSoft = main.attr("main")();
    std::wstring title = mainSoft.attr("title").cast<std::wstring>();
    if (title.length() > 30)
    {
        return 1;
    }
    else if (!title.empty())
    {
        wchar_t titleBuffer[31];
        wcscpy_s(titleBuffer, title.c_str());
        SetWindowTextW(hWnd, titleBuffer);
    }

    rootElement = mainSoft.attr("home")().attr("get_element").cast<py::dict>();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}
