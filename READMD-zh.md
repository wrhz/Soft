# Soft · [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

为 Python 赋予原生性能的跨平台声明式 UI 框架

> “我不是在封装底层，我是在重新定义接口。”  
> —— 一位 12 岁的系统架构师

## 🚀 为什么选择 Soft？

| 特性 | Soft | 其他 Python GUI 框架 |
|------|------|---------------------|
| 架构模式 | 底层驱动上层（C++ 主循环调用 Python 组件） | 上层调用底层（Python 调用 C 扩展） |
| 性能 | 直接系统 API 调用，零中间层损耗 | 通常有解释器或抽象层开销 |
| 跨平台一致性 | 分平台原生实现（Win32、GTK、Android NDK） | 依赖跨平台抽象库（如 wxWidgets、Qt 绑定） |
| 声明式语法 | ✅ Flutter-inspired 组件树 | ❌ 通常为命令式 |
| 数学/数据科学集成 | ✅ 深度预留 NumPy、Matplotlib 接口 | ❌ 通常仅 UI 功能 |

如果你厌倦了 Python GUI 框架的性能瓶颈、复杂配置或跨平台表现不一致——Soft 为你提供一条全新的路径。

## ✨ 核心设计哲学

### 1. 逆向控制架构
- **传统框架**：Python → 框架 → 系统 API  
- **Soft 架构**：C++ 主循环 → Python 声明式组件 → C++ 直接渲染  

这意味着：
- **性能掌握在框架手中**：渲染循环完全在 C++ 侧运行
- **用户代码不可“撒野”**：Python 侧仅提供 UI 描述，无法破坏渲染管线
- **极致优化可能**：可针对不同平台进行汇编级优化

### 2. 分平台原生实现
```cpp
// platforms/windows/renderer_win.cpp
class WindowsRenderer : public Renderer {
    void drawText(...) override { /* Win32 GDI/DirectWrite */ }
};

// platforms/linux/renderer_x11.cpp  
class LinuxRenderer : public Renderer {
    void drawText(...) override { /* X11/XCB + Cairo/Pango */ }
};