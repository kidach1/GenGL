#include "application.h"
#include <iostream>
#include <stdexcept>
#include <glad/gl.h>
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
    : window(nullptr), running(false), currentTime(0.0f), lastTime(0.0f), deltaTime(0.0f),
      shader(nullptr), model(nullptr), rotationSpeed(1.0f) {
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
        
        // シェーダーの初期化
        shader = std::make_unique<Shader>();
        if (!shader->loadFromFile("assets/shaders/basic.vs", "assets/shaders/basic.fs")) {
            std::cerr << "Failed to load shaders" << std::endl;
            return false;
        }
        
        // モデルのロード
        try {
            model = std::make_unique<Model>("assets/models/teapot.obj");
        } catch (const std::exception& e) {
            std::cerr << "Failed to load model: " << e.what() << std::endl;
            return false;
        }
        
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
    
    // OpenGLリソースの解放
    model.reset(); // モデルを先に解放（依存関係のため）
    shader.reset();
    
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
    // アプリケーションの状態更新
    
    // モデルの回転（Y軸周り）
    if (model) {
        model->rotateY(rotationSpeed * deltaTime);
    }
}

void Application::render() {
    // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (shader && window && model) {
        // シェーダーを使用
        shader->use();
        
        // カメラとライトの設定
        glm::vec3 viewPos(0.0f, 0.0f, 10.0f); // teapot表示用にカメラを適切な距離に配置
        glm::vec3 lightPos(1.0f, 1.0f, 2.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 objectColor(0.5f, 0.5f, 0.5f);
        
        // ライト関連の設定（Phongシェーディング用）
        shader->setVec3("viewPos", viewPos);
        shader->setVec3("lightPos", lightPos);
        shader->setVec3("lightColor", lightColor);
        shader->setVec3("objectColor", objectColor);
        shader->setFloat("ambientStrength", 0.1f);
        shader->setFloat("specularStrength", 0.5f);
        shader->setInt("shininess", 32);
        
        // 変換行列設定
        glm::mat4 view = glm::lookAt(
            viewPos,                      // カメラ位置
            glm::vec3(0.0f, 0.0f, 0.0f),  // 注視点
            glm::vec3(0.0f, 1.0f, 0.0f)   // 上方向
        );
        
        // 投影行列（透視投影）
        float aspectRatio = window->getAspectRatio();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
        
        // Uniform設定
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        
        // モデルの描画
        
        // teapotの設定調整 - 初期回転を加えて風景を見やすくする
        glm::mat4 initialTransform = glm::mat4(1.0f);
        // X軸に少し回転してティーポットの口が見えるようにする
        initialTransform = glm::rotate(initialTransform, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        // 現在のモデル行列と組み合わせる
        glm::mat4 modelMatrix = initialTransform * model->getModelMatrix();
        shader->setMat4("model", modelMatrix);
        
        // モデル描画
        model->draw(*shader);
    }
}

} // namespace claude_gl
