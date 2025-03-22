#pragma once

#include <string>
#include <functional>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace claude_gl {

/**
 * @brief OpenGLウィンドウを管理するクラス
 * 
 * GLFWを使用してウィンドウを作成し、イベント処理を行う
 */
class Window {
public:
    /**
     * @brief Window クラスのコンストラクタ
     * @param width ウィンドウの幅
     * @param height ウィンドウの高さ
     * @param title ウィンドウのタイトル
     */
    Window(int width, int height, const std::string& title);
    
    /**
     * @brief デストラクタ
     */
    ~Window();
    
    /**
     * @brief ウィンドウとOpenGLコンテキストを初期化する
     * @return 初期化が成功したかどうか
     */
    bool initialize();
    
    /**
     * @brief ウィンドウとOpenGLリソースを解放する
     */
    void shutdown();
    
    /**
     * @brief ウィンドウの状態を更新する
     */
    void update();
    
    /**
     * @brief ウィンドウが閉じられるべきかどうかを確認する
     * @return ウィンドウが閉じられるべきならtrue
     */
    bool shouldClose() const;
    
    /**
     * @brief ウィンドウが閉じられるべきかどうかを設定する
     * @param shouldClose ウィンドウが閉じられるべきならtrue
     */
    void setShouldClose(bool shouldClose);
    
    /**
     * @brief バックバッファとフロントバッファを入れ替える
     */
    void swapBuffers();
    
    /**
     * @brief ウィンドウのサイズを設定する
     * @param width 新しい幅
     * @param height 新しい高さ
     */
    void setSize(int width, int height);
    
    /**
     * @brief ウィンドウのサイズを取得する
     * @param width 幅を格納する変数への参照
     * @param height 高さを格納する変数への参照
     */
    void getSize(int& width, int& height) const;
    
    /**
     * @brief ウィンドウのアスペクト比を取得する
     * @return アスペクト比（幅 / 高さ）
     */
    float getAspectRatio() const;
    
    /**
     * @brief フルスクリーンモードを設定する
     * @param fullscreen フルスクリーンモードならtrue
     */
    void setFullscreen(bool fullscreen);
    
    /**
     * @brief フルスクリーンモードかどうかを取得する
     * @return フルスクリーンモードならtrue
     */
    bool isFullscreen() const;
    
    /**
     * @brief GLFWウィンドウハンドルを取得する
     * @return GLFWウィンドウハンドル
     */
    GLFWwindow* getHandle() const;
    
    /**
     * @brief フレームバッファサイズ変更コールバックを設定する
     * @param callback コールバック関数
     */
    void setFramebufferSizeCallback(std::function<void(int, int)> callback);
    
private:
    GLFWwindow* window;          ///< GLFWウィンドウハンドル
    int width;                   ///< ウィンドウの幅
    int height;                  ///< ウィンドウの高さ
    std::string title;           ///< ウィンドウのタイトル
    bool fullscreen;             ///< フルスクリーンモードかどうか
    
    std::function<void(int, int)> framebufferSizeCallback; ///< フレームバッファサイズ変更コールバック
    
    static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
    static void windowCloseCallbackWrapper(GLFWwindow* window);
};

} // namespace claude_gl
