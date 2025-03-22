# Claude-OpenGL プロジェクト コンテキスト

## プロジェクト概要

OpenGLを使用した3Dグラフィックスアプリケーションの開発プロジェクト。GLFW, GLAD, GLMライブラリを使用して、モダンなOpenGLプログラミングを学習・実践することを目的としています。

## 使用ライブラリ

- **GLAD**: OpenGL関数ポインタのロードを担当
- **GLFW**: ウィンドウ作成とOpenGLコンテキスト管理
- **GLM**: OpenGLの数学ライブラリ（行列、ベクトル、変換）

## ディレクトリ構造

```
Claude-OpenGL/
├── .clauderules            # プロジェクト規則とコーディングスタイル
├── CMakeLists.txt          # CMakeビルド設定
├── src/                    # ソースコード
│   ├── main.cpp            # メインエントリーポイント
│   ├── core/               # コア機能
│   │   ├── application.cpp # アプリケーション管理
│   │   ├── application.h
│   │   ├── window.cpp      # ウィンドウ管理（GLFW利用）
│   │   └── window.h
│   ├── renderer/           # レンダリング関連コード
│   └── utils/              # ユーティリティ
├── include/                # ヘッダーファイル
├── assets/                 # アセット（シェーダー、テクスチャなど）
│   ├── shaders/
│   └── textures/
└── libs/                   # 外部ライブラリ
    ├── glad-3.3/           # GLAD 3.3（OpenGL 3.3 Core Profile）
    ├── glfw/               # GLFW
    └── glm/                # GLM（OpenGL Mathematics）
```

## 実装状況

1. **基本設定**:
   - プロジェクト構造とCMake設定
   - コーディング規約（.clauderules）

2. **コアシステム**:
   - Window クラス: GLFW ラッパー
   - Application クラス: シングルトン、メインループ管理

3. **レンダリング**:
   - 基本的なOpenGLコンテキスト初期化
   - Shader クラス: シェーダーの読み込み、コンパイル、使用
   - 三角形の描画（頂点と色の属性）

## 発生した問題と解決策

### 1. GLADの初期化エラー
**問題**: GLAD 3.3のAPIが変更されていた  
**解決**: 初期化関数を`gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)`から`gladLoadGL(glfwGetProcAddress)`に更新

### 2. macOSでのGLFWライブラリ読み込みエラー
**問題**: macOSのセキュリティポリシーにより、動的ライブラリ（.dylib）のロードが拒否された  
**解決**: 静的ライブラリ（.a）を使用するように変更し、必要なmacOSフレームワーク（Cocoa, OpenGL, IOKit, CoreVideo）を追加

## ビルド方法

```bash
# プロジェクトのルートディレクトリから
mkdir -p build
cd build
cmake ..
make
```

## 実行方法

```bash
# buildディレクトリから
./Claude-OpenGL
```

## 次のステップ

1. ~~Shader クラスの実装~~ ✓
2. ~~基本的な三角形の描画~~ ✓
3. Mesh クラスの実装
4. Texture クラスの実装
5. Camera システムの実装

## Git コミット履歴

1. プロジェクト基盤の実装 (Window, Application クラス)
2. GLAD初期化関数の更新
3. GLFWの静的ライブラリ利用への変更
4. [レンダリング] シェーダークラス実装と基本三角形描画
