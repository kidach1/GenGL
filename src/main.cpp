#include <iostream>
#include "core/application.h"

int main(int argc, char* argv[]) {
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    const std::string WINDOW_TITLE = "Claude OpenGL";
    
    try {
        // アプリケーションの取得と初期化
        claude_gl::Application& app = claude_gl::Application::getInstance();
        
        if (!app.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)) {
            std::cerr << "Failed to initialize application" << std::endl;
            return -1;
        }
        
        // メインループの実行
        app.run();
        
        // 正常終了
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
}
