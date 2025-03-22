#include "application.h"
#include <iostream>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace claude_gl {

// 静的メンバ変数の定義
Application* Application::instance = nullptr;

Application& Application::getInstance() {
    if (!instance) {
        instance = new Application();
    }
    return *instance;
}

Application::Application()
    : window(nullptr), running(false), currentTime(0.0f), lastTime(0.0f), deltaTime(0.0f) {
}

Application::~Application() {
    shutdown();
}

bool Application::initialize(int width, int height, const std::string& title) {
    try {
        // ウィンドウの作成と初期化
        window = std::make_unique<Window>(width, height, title);
        if (!window->initialize()) {
            std::cerr << "Failed to initialize window" << std::endl;
            return false;
        }
        
        // フレームバッファサイズ変更コールバックの設定
        window->setFramebufferSizeCallback([](int width, int height) {
            glViewport(0, 0, width, height);
        });
        
        // OpenGL設定の初期化
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        
        // 時間の初期化
        lastTime = static_cast<float>(glfwGetTime());
        currentTime = lastTime;
        deltaTime = 0.0f;
        
        running = true;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception during initialization: " << e.what() << std::endl;
        return false;
    }
}

void Application::run() {
    if (!window) {
        std::cerr << "Cannot run application without initializing window first" << std::endl;
        return;
    }
    
    // メインループ
    while (running && !window->shouldClose()) {
        // 時間の更新
        currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // 入力処理、更新、描画
        processInput();
        update();
        render();
        
        // バッファのスワップと入力イベントの処理
        window->swapBuffers();
        window->update();
    }
}

void Application::shutdown() {
    running = false;
    
    if (window) {
        window->shutdown();
        window.reset();
    }
    
    // シングルトンインスタンスのクリーンアップ
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void Application::setRunning(bool running) {
    this->running = running;
}

bool Application::isRunning() const {
    return running;
}

float Application::getDeltaTime() const {
    return deltaTime;
}

float Application::getTime() const {
    return currentTime;
}

Window* Application::getWindow() {
    return window.get();
}

void Application::processInput() {
    // ESCキーでアプリケーション終了
    if (window && glfwGetKey(window->getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        window->setShouldClose(true);
    }
}

void Application::update() {
    // アプリケーションの状態更新（サブシステムの更新）
    // 現時点では実装はまだありません
}

void Application::render() {
    // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // レンダリング処理
    // 現時点では実装はまだありません
}

} // namespace claude_gl
