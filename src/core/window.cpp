#include "window.h"
#include <iostream>
#include <stdexcept>

namespace claude_gl {

// 静的メンバ関数定義
void Window::framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height) {
    Window* userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (userWindow && userWindow->framebufferSizeCallback) {
        userWindow->framebufferSizeCallback(width, height);
    }
}

void Window::windowCloseCallbackWrapper(GLFWwindow* window) {
    Window* userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (userWindow) {
        userWindow->setShouldClose(true);
    }
}

Window::Window(int width, int height, const std::string& title)
    : window(nullptr), width(width), height(height), title(title), fullscreen(false) {
}

Window::~Window() {
    shutdown();
}

bool Window::initialize() {
    // GLFWの初期化
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    // OpenGLバージョンとプロファイルの設定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    // ウィンドウの作成
    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    // 現在のコンテキストに設定
    glfwMakeContextCurrent(window);
    
    // GLADの初期化（OpenGL関数ポインタのロード）
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }
    
    // ウィンドウユーザーポインタの設定（コールバック内でthisポインタにアクセスするため）
    glfwSetWindowUserPointer(window, this);
    
    // コールバックの設定
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackWrapper);
    glfwSetWindowCloseCallback(window, windowCloseCallbackWrapper);
    
    // 垂直同期の有効化
    glfwSwapInterval(1);
    
    // ビューポートの設定
    glViewport(0, 0, width, height);
    
    return true;
}

void Window::shutdown() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void Window::update() {
    glfwPollEvents();
}

bool Window::shouldClose() const {
    return window ? glfwWindowShouldClose(window) : true;
}

void Window::setShouldClose(bool shouldClose) {
    if (window) {
        glfwSetWindowShouldClose(window, shouldClose);
    }
}

void Window::swapBuffers() {
    if (window) {
        glfwSwapBuffers(window);
    }
}

void Window::setSize(int width, int height) {
    this->width = width;
    this->height = height;
    if (window) {
        glfwSetWindowSize(window, width, height);
    }
}

void Window::getSize(int& width, int& height) const {
    width = this->width;
    height = this->height;
    
    if (window) {
        glfwGetWindowSize(window, &width, &height);
    }
}

float Window::getAspectRatio() const {
    return static_cast<float>(width) / static_cast<float>(height);
}

void Window::setFullscreen(bool fullscreen) {
    if (this->fullscreen == fullscreen) {
        return;
    }
    
    this->fullscreen = fullscreen;
    
    if (window) {
        // 現在のウィンドウの位置とサイズを保存
        int posX, posY;
        glfwGetWindowPos(window, &posX, &posY);
        
        // 現在のウィンドウを破棄
        glfwDestroyWindow(window);
        
        // 新しいウィンドウの作成
        GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
        
        if (fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            width = mode->width;
            height = mode->height;
        }
        
        window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
        if (!window) {
            std::cerr << "Failed to recreate GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        
        // ウィンドウモードの場合は前の位置に戻す
        if (!fullscreen) {
            glfwSetWindowPos(window, posX, posY);
        }
        
        // 現在のコンテキストに設定
        glfwMakeContextCurrent(window);
        
        // コールバックの再設定
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackWrapper);
        glfwSetWindowCloseCallback(window, windowCloseCallbackWrapper);
        
        // 垂直同期の再設定
        glfwSwapInterval(1);
        
        // ビューポートの再設定
        glViewport(0, 0, width, height);
    }
}

bool Window::isFullscreen() const {
    return fullscreen;
}

GLFWwindow* Window::getHandle() const {
    return window;
}

void Window::setFramebufferSizeCallback(std::function<void(int, int)> callback) {
    framebufferSizeCallback = callback;
}

} // namespace claude_gl
