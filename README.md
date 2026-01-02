# Soft · [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A Cross-Platform Declarative UI Framework Bringing Native Performance to Python

> "I'm not wrapping the underlying layer; I'm redefining the interface."  
> — A 12-year-old system architect

## 🚀 Why Choose Soft?

| Feature | Soft | Other Python GUI Frameworks |
|------|------|---------------------|
| Architecture Pattern | Bottom layer drives top (C++ main loop calls Python components) | Top layer calls bottom (Python calls C extensions) |
| Performance | Direct system API calls, zero intermediate layer overhead | Typically has interpreter or abstraction layer overhead |
| Cross-Platform Consistency | Platform-native implementations (Win32, X11/Wayland, Android NDK) | Relies on cross-platform abstraction libraries (e.g., wxWidgets, Qt bindings) |
| Declarative Syntax | ✅ Flutter-inspired component tree | ❌ Typically imperative |
| Math/Data Science Integration | ✅ Deep NumPy, Matplotlib interface integration | ❌ Typically UI-only functionality |

If you're tired of Python GUI frameworks' performance bottlenecks, complex configuration, or inconsistent cross-platform behavior—Soft offers a brand new path.

## ✨ Core Design Philosophy

### 1. Inverse Control Architecture
- **Traditional Frameworks**: Python → Framework → System API  
- **Soft Architecture**: C++ Main Loop → Python Declarative Components → C++ Direct Rendering  

This means:
- **Performance controlled by the framework**: The rendering loop runs entirely on the C++ side
- **User code cannot "run wild"**: The Python side only provides UI descriptions, cannot break the rendering pipeline
- **Ultimate optimization potential**: Assembly-level optimizations possible for different platforms

### 2. Platform-Native Implementations
```cpp
// platforms/windows/renderer_win.cpp
class WindowsRenderer : public Renderer {
    void drawText(...) override { /* Win32 GDI/DirectWrite */ }
};

// platforms/linux/renderer_x11.cpp  
class LinuxRenderer : public Renderer {
    void drawText(...) override { /* X11/XCB + Cairo/Pango */ }
};
