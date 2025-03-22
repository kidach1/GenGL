#pragma once

#include <memory>
#include <string>
#include "window.h"

namespace claude_gl {

/**
 * @brief アプリケーション全体を管理するクラス
 * 
 * シングルトンパターンを使用してアプリケーションのライフサイクルとメインループを管理する
 */
class Application {
public:
    /**
     * @brief アプリケーションのシングルトンインスタンスを取得する
     * @return アプリケーションのインスタンス
     */
    static Application& getInstance();
    
    /**
     * @brief デストラクタ
     */
    ~Application();
    
    /**
     * @brief アプリケーションを初期化する
     * @param width ウィンドウの幅
     * @param height ウィンドウの高さ
     * @param title ウィンドウのタイトル
     * @return 初期化が成功したかどうか
     */
    bool initialize(int width, int height, const std::string& title);
    
    /**
     * @brief アプリケーションのメインループを実行する
     */
    void run();
    
    /**
     * @brief アプリケーションをシャットダウンする
     */
    void shutdown();
    
    /**
     * @brief アプリケーションの実行状態を設定する
     * @param running 実行中ならtrue
     */
    void setRunning(bool running);
    
    /**
     * @brief アプリケーションが実行中かどうかを取得する
     * @return 実行中ならtrue
     */
    bool isRunning() const;
    
    /**
     * @brief 前回のフレームからの経過時間を取得する
     * @return 経過時間（秒）
     */
    float getDeltaTime() const;
    
    /**
     * @brief アプリケーションの開始からの経過時間を取得する
     * @return 経過時間（秒）
     */
    float getTime() const;
    
    /**
     * @brief ウィンドウオブジェクトを取得する
     * @return ウィンドウオブジェクトへのポインタ
     */
    Window* getWindow();
    
private:
    /**
     * @brief プライベートコンストラクタ（シングルトンパターン）
     */
    Application();
    
    /**
     * @brief コピーコンストラクタ（禁止）
     */
    Application(const Application&) = delete;
    
    /**
     * @brief 代入演算子（禁止）
     */
    Application& operator=(const Application&) = delete;
    
    /**
     * @brief 入力処理を行う
     */
    void processInput();
    
    /**
     * @brief アプリケーションの状態を更新する
     */
    void update();
    
    /**
     * @brief 描画処理を行う
     */
    void render();
    
    static Application* instance;     ///< シングルトンインスタンス
    
    std::unique_ptr<Window> window;   ///< ウィンドウオブジェクト
    
    bool running;                     ///< アプリケーションが実行中かどうか
    float currentTime;                ///< 現在の時間
    float lastTime;                   ///< 前回のフレームの時間
    float deltaTime;                  ///< 前回のフレームからの経過時間
};

} // namespace claude_gl
