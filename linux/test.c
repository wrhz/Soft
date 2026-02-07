#include <wayland-client.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

// 简化结构体
struct app_state {
    // Wayland连接
    struct wl_display *display;
    struct wl_registry *registry;
    
    // Wayland全局对象
    struct wl_compositor *compositor;
    struct wl_shell *shell;
    struct wl_shm *shm;
    
    // 窗口相关
    struct wl_surface *surface;
    struct wl_shell_surface *shell_surface;
    
    // 窗口尺寸
    int width, height;
    
    // 运行状态
    int running;
};

// ==================== 共享内存辅助函数 ====================

static int create_shm_file(void) {
    char template[] = "/tmp/wayland-shm-XXXXXX";
    int fd = mkstemp(template);
    if (fd < 0) {
        return -1;
    }
    unlink(template);
    return fd;
}

struct wl_buffer *create_buffer(struct app_state *state, int width, int height, void **data_ptr) {
    int stride = width * 4; // ARGB8888格式
    int size = stride * height;
    
    // 创建共享内存文件
    int fd = create_shm_file();
    if (fd < 0) {
        fprintf(stderr, "创建共享内存文件失败\n");
        return NULL;
    }
    
    // 设置文件大小
    if (ftruncate(fd, size) < 0) {
        fprintf(stderr, "设置共享内存大小失败\n");
        close(fd);
        return NULL;
    }
    
    // 映射内存
    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        fprintf(stderr, "内存映射失败\n");
        close(fd);
        return NULL;
    }
    
    // 创建wl_shm_pool
    struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, size);
    
    // 创建wl_buffer
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
                                                       width, height,
                                                       stride,
                                                       WL_SHM_FORMAT_ARGB8888);
    
    // 清理
    wl_shm_pool_destroy(pool);
    close(fd);
    
    if (data_ptr) {
        *data_ptr = data;
    }
    
    return buffer;
}

// ==================== 绘图函数 ====================

static void draw_color(void *data, int width, int height, uint32_t color) {
    uint32_t *pixel = (uint32_t *)data;
    for (int i = 0; i < width * height; i++) {
        pixel[i] = color;
    }
}

static void draw_checkerboard(void *data, int width, int height, int frame) {
    uint32_t *pixel = (uint32_t *)data;
    int square_size = 20;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int xx = x + frame % square_size;
            int yy = y + frame % square_size;
            
            pixel[y * width + x] = 0xFFFFFFFF;
        }
    }
}

// ==================== Wayland监听器 ====================

// shell_surface监听器
static void shell_surface_ping(void *data,
    struct wl_shell_surface *shell_surface,
    uint32_t serial) {
    wl_shell_surface_pong(shell_surface, serial);
}

static void shell_surface_configure(void *data,
    struct wl_shell_surface *shell_surface,
    uint32_t edges,
    int32_t width,
    int32_t height) {
    struct app_state *state = data;
    
    if (width > 0 && height > 0) {
        state->width = width;
        state->height = height;
        printf("窗口大小改变: %dx%d\n", width, height);
    }
}

static void shell_surface_popup_done(void *data,
    struct wl_shell_surface *shell_surface) {
}

static const struct wl_shell_surface_listener shell_surface_listener = {
    .ping = shell_surface_ping,
    .configure = shell_surface_configure,
    .popup_done = shell_surface_popup_done,
};

// buffer监听器
static void buffer_release(void *data, struct wl_buffer *buffer) {
    // 缓冲区释放时不做特别处理
}

static const struct wl_buffer_listener buffer_listener = {
    .release = buffer_release,
};

// registry监听器
static void registry_global(void *data,
    struct wl_registry *registry,
    uint32_t name,
    const char *interface,
    uint32_t version) {
    
    struct app_state *state = data;
    
    if (strcmp(interface, "wl_compositor") == 0) {
        state->compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 1);
        printf("获取 compositor\n");
    } else if (strcmp(interface, "wl_shell") == 0) {
        state->shell = wl_registry_bind(registry, name, &wl_shell_interface, 1);
        printf("获取 shell\n");
    } else if (strcmp(interface, "wl_shm") == 0) {
        state->shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
        printf("获取 shm (共享内存)\n");
    }
}

static void registry_global_remove(void *data,
    struct wl_registry *registry,
    uint32_t name) {
    // 全局对象被移除
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

// ==================== 主函数 ====================

int main(int argc, char **argv) {
    struct app_state state = {0};
    state.width = 400;
    state.height = 300;
    state.running = 1;
    
    printf("Wayland 窗口演示程序\n");
    printf("==================\n");
    
    // 1. 连接到Wayland显示服务器
    printf("连接到Wayland服务器...\n");
    state.display = wl_display_connect(NULL);
    if (!state.display) {
        fprintf(stderr, "错误: 无法连接到Wayland显示服务器\n");
        fprintf(stderr, "提示: 确保你在Wayland环境中运行\n");
        fprintf(stderr, "     或启动Weston合成器: weston &\n");
        return 1;
    }
    printf("连接成功\n");
    
    // 2. 获取注册表
    printf("获取注册表...\n");
    state.registry = wl_display_get_registry(state.display);
    wl_registry_add_listener(state.registry, &registry_listener, &state);
    
    // 3. 同步获取全局对象
    printf("获取全局接口...\n");
    wl_display_roundtrip(state.display);
    
    // 检查必需的接口
    if (!state.compositor) {
        fprintf(stderr, "错误: 没有获取到compositor\n");
        return 1;
    }
    
    if (!state.shell) {
        fprintf(stderr, "错误: 没有获取到shell\n");
        return 1;
    }
    
    if (!state.shm) {
        fprintf(stderr, "错误: 没有获取到shm\n");
        return 1;
    }
    
    // 4. 创建表面
    printf("创建窗口表面...\n");
    state.surface = wl_compositor_create_surface(state.compositor);
    if (!state.surface) {
        fprintf(stderr, "错误: 无法创建表面\n");
        return 1;
    }
    
    // 5. 创建shell表面
    printf("创建shell表面...\n");
    state.shell_surface = wl_shell_get_shell_surface(state.shell, state.surface);
    if (!state.shell_surface) {
        fprintf(stderr, "错误: 无法创建shell表面\n");
        return 1;
    }
    
    // 设置监听器
    wl_shell_surface_add_listener(state.shell_surface, &shell_surface_listener, &state);
    
    // 设置表面类型和标题
    wl_shell_surface_set_toplevel(state.shell_surface);
    wl_shell_surface_set_title(state.shell_surface, "Wayland测试窗口");
    
    printf("初始化完成！窗口已创建\n");
    printf("窗口大小: %dx%d\n", state.width, state.height);
    printf("按Ctrl+C退出程序\n\n");
    
    // 创建初始缓冲区
    void *buffer_data = NULL;
    struct wl_buffer *buffer = create_buffer(&state, state.width, state.height, &buffer_data);
    if (!buffer) {
        fprintf(stderr, "错误: 无法创建缓冲区\n");
        return 1;
    }
    
    // 添加缓冲区监听器
    wl_buffer_add_listener(buffer, &buffer_listener, NULL);
    
    // 绘制初始内容
    draw_checkerboard(buffer_data, state.width, state.height, 0);
    
    // 附加缓冲区到表面
    wl_surface_attach(state.surface, buffer, 0, 0);
    
    // 设置损坏区域（整个窗口）
    wl_surface_damage(state.surface, 0, 0, state.width, state.height);
    
    // 提交表面
    wl_surface_commit(state.surface);
    
    printf("第一帧已渲染\n");
    
    // 6. 简单的事件循环
    int frame = 0;
    while (state.running) {
        // 处理事件
        wl_display_dispatch_pending(state.display);
        
        // 简单动画：每10次循环更新一次
        if (frame % 10 == 0) {
            draw_checkerboard(buffer_data, state.width, state.height, frame / 10);
            
            // 重新附加缓冲区（双缓冲效果）
            wl_surface_attach(state.surface, buffer, 0, 0);
            wl_surface_damage(state.surface, 0, 0, state.width, state.height);
            wl_surface_commit(state.surface);
            
            printf("更新帧: %d\n", frame / 10);
        }
        
        // 读取事件（非阻塞）
        wl_display_flush(state.display);
        int ret = wl_display_dispatch(state.display);
        if (ret == -1) {
            printf("显示服务器连接断开\n");
            break;
        }
        
        frame++;
        usleep(10000); // 睡眠10毫秒
    }
    
    // 7. 清理资源
    printf("\n正在清理资源...\n");
    
    // 释放共享内存
    if (buffer_data) {
        munmap(buffer_data, state.width * state.height * 4);
    }
    
    if (buffer) {
        wl_buffer_destroy(buffer);
    }
    
    if (state.shell_surface) {
        wl_shell_surface_destroy(state.shell_surface);
    }
    
    if (state.surface) {
        wl_surface_destroy(state.surface);
    }
    
    if (state.shell) {
        wl_shell_destroy(state.shell);
    }
    
    if (state.shm) {
        wl_shm_destroy(state.shm);
    }
    
    if (state.compositor) {
        wl_compositor_destroy(state.compositor);
    }
    
    if (state.registry) {
        wl_registry_destroy(state.registry);
    }
    
    if (state.display) {
        wl_display_disconnect(state.display);
    }
    
    printf("程序正常结束\n");
    return 0;
}