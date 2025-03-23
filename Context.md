# Claude-OpenGL プロジェクト ドキュメント

## 概要
このドキュメントは人間向けのプロジェクト説明と進行状況の記録です。
プロジェクトの実装に関わるAIアシスタントのためのルールは .clauderules ファイルに記載されています。

## プロジェクト概要

OpenGLを使用した3Dグラフィックスアプリケーションの開発プロジェクト。GLFW, GLAD, GLMライブラリを使用して、モダンなOpenGLプログラミングを学習・実践することを目的としています。本プロジェクトではClaudeによるコード生成と実装を活用し、段階的に機能を拡張していきます。

## 使用ライブラリ

- **GLAD**: OpenGL関数ポインタのロードを担当（OpenGL 3.3 Core Profile対応）
- **GLFW**: ウィンドウ作成とOpenGLコンテキスト管理（クロスプラットフォーム対応）
- **GLM**: OpenGLの数学ライブラリ（行列、ベクトル、変換）

## ディレクトリ構造

```
Claude-OpenGL/
├── .clauderules              # AI実装のためのルール定義
├── Context.md                # プロジェクト状況の記録（このファイル）
├── CMakeLists.txt            # CMakeビルド設定
├── build_windows.bat         # Windows用ビルドスクリプト
├── build_linux.sh            # Linux用ビルドスクリプト
├── src/                      # ソースコード
│   ├── main.cpp              # メインエントリーポイント
│   ├── core/                 # コア機能
│   │   ├── application.cpp   # アプリケーション管理
│   │   ├── application.h
│   │   ├── window.cpp        # ウィンドウ管理（GLFW利用）
│   │   └── window.h
│   ├── renderer/             # レンダリング関連コード
│   │   ├── mesh.cpp          # メッシュ管理
│   │   ├── model.cpp         # 3Dモデル管理
│   │   ├── shader.cpp        # シェーダー管理
│   │   └── shader.h
│   └── utils/                # ユーティリティ
│       └── platform_utils.h  # プラットフォーム互換性ユーティリティ
├── include/                  # 公開ヘッダーファイル
│   └── renderer/             # レンダリング関連ヘッダー
│       ├── mesh.h            # メッシュ管理ヘッダー
│       └── model.h           # モデル管理ヘッダー
├── assets/                   # アセット（シェーダー、テクスチャなど）
│   ├── models/               # 3Dモデルファイル
│   │   ├── cube.obj          # キューブモデル
│   │   └── teapot.obj        # ティーポットモデル
│   ├── shaders/              # シェーダープログラム
│   │   ├── basic.vs          # 基本的な頂点シェーダー
│   │   └── basic.fs          # 基本的なフラグメントシェーダー
│   └── textures/             # テクスチャファイル
└── libs/                     # 外部ライブラリ
    ├── glad-3.3/             # GLAD 3.3
    ├── glfw/                 # GLFW
    │   ├── include/          # GLFWヘッダー
    │   ├── lib-arm64/        # macOS M1/M2/M3用ライブラリ
    │   ├── lib-x86_64/       # macOS Intel用ライブラリ
    │   ├── lib-win32/        # Windows 32bit用ライブラリ
    │   ├── lib-win64/        # Windows 64bit用ライブラリ
    │   ├── lib-linux/        # Linux用ライブラリ
    │   ├── bin-win32/        # Windows 32bit DLL
    │   └── bin-win64/        # Windows 64bit DLL
    └── glm/                  # GLM
```

## 実装状況と進捗状況

### 1. 基本設定 (完了)
- プロジェクト構造とCMake設定
- 外部ライブラリ（GLAD, GLFW, GLM）の統合

### 2. コアシステム (完了)
- **Window クラス**: GLFW ラッパー
  - ウィンドウ作成と管理
  - イベント処理
  - フルスクリーン切り替え
- **Application クラス**: シングルトン、メインループ管理
  - 初期化処理
  - メインループ（update、render）
  - タイミング管理

### 3. レンダリングシステム (部分完了)
- **Shader クラス**: シェーダープログラムの管理 (完了)
  - ファイルからのシェーダー読み込み
  - シェーダーのコンパイルとリンク
  - Uniform変数の設定（各種型対応）
- **Mesh クラス**: メッシュデータの管理 (完了)
  - 頂点属性（位置、法線、UV座標）の管理
  - インデックスバッファ対応
  - VAO、VBO、EBOの自動管理
- **Model クラス**: 3Dモデルの管理 (完了)
  - OBJファイルの読み込み
  - 複数メッシュの管理
  - モデル変換（回転など）
- **基本レンダリング**: (完了)
  - 三角形の描画（頂点と色の属性）
  - 変換行列の適用（モデル・ビュー・プロジェクション）
  - OBJモデルのレンダリング
- **注意点**:
  - 現時点ではレンダリングコードがApplicationクラスに配置されています
  - 将来的に専用Rendererクラスに移行予定

### 4. マルチプラットフォーム対応 (進行中)
- **CMake設定の拡張**:
  - Windows、Linux、macOS向けの条件分岐
  - プラットフォーム固有のライブラリパス設定
  - システムパッケージのフォールバック対応
- **ビルドスクリプト**:
  - Windows向けバッチファイル
  - Linux向けシェルスクリプト
- **プラットフォームユーティリティ**:
  - パス操作の互換性対応
  - 実行ファイルパス取得のプラットフォーム依存コード
  - アセットパス解決

## 開発上の問題と解決策

### 1. GLADの初期化エラー
**問題**: GLAD 3.3のAPIが変更されていた  
**解決**: 初期化関数を`gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)`から`gladLoadGL(glfwGetProcAddress)`に更新

### 2. macOSでのGLFWライブラリ読み込みエラー
**問題**: macOSのセキュリティポリシーにより、動的ライブラリ（.dylib）のロードが拒否された  
**解決**: 静的ライブラリ（.a）を使用するように変更し、必要なmacOSフレームワーク（Cocoa, OpenGL, IOKit, CoreVideo）を追加

### 3. レンダリングコードの一時的配置
**問題**: レンダリングコードを最終的にはRenderer専用クラスに移動したいが、開発初期段階で全てのアーキテクチャを構築するのは非効率  
**対応**: 一時的にApplicationクラス内でレンダリングコードを実装  
**将来計画**: 次の実装段階でRendererクラスを専用に設計し、現在のレンダリングコードを移行予定

### 4. マルチプラットフォーム対応
**問題**: 元のCMake設定がmacOS中心で、Windows/Linuxでのビルドに対応していなかった  
**対応**:
- CMakeを拡張してプラットフォームごとのライブラリパスと依存関係を設定
- プラットフォーム固有のコードを条件付きコンパイルで分離
- ファイルパス操作をプラットフォーム非依存に実装

## ビルド方法

### macOS/Linux
```bash
# プロジェクトのルートディレクトリから
# Linuxの場合はビルドスクリプトを実行権限付きにする必要があります
chmod +x build_linux.sh

# ビルドスクリプト実行
./build_linux.sh
```

### Windows
```bat
REM コマンドプロンプトから
build_windows.bat
```

または、Visual Studioで直接CMakeプロジェクトを開いてビルドすることも可能です。

## 実行方法

### macOS/Linux
```bash
# buildディレクトリから
./Claude-OpenGL
```

### Windows
```bat
REM buildディレクトリから
Release\Claude-OpenGL.exe
```

## 次の実装計画

### 短期的なタスク
1. ~~Shader クラスの実装~~ ✓
2. ~~基本的な三角形の描画~~ ✓
3. ~~Mesh クラスの実装~~ ✓
4. ~~Model クラスの実装~~ ✓
5. ~~マルチプラットフォーム対応~~ (進行中)
6. Texture クラスの実装
   - ファイルからの画像読み込み
   - ミップマップ生成とフィルタ設定
   - 複数テクスチャの管理

### 中期的な目標
7. Camera システムの実装
   - 自由視点カメラの実装
   - 透視投影と平行投影のサポート
   - マウス入力でのカメラ操作
8. Rendererクラスの設計
   - Applicationからのレンダリングコードの分離
   - レンダリングパイプラインの抽象化
   - 描画コマンドのバッチ化

## 中長期的な実装計画

1. **基本的なシーングラフ**
   - 変換階層の実装
   - 親子関係の管理

2. **高度なシェーディング**
   - Phongシェーディング
   - マテリアルシステム
   - 複数光源サポート

3. **システム拡張**
   - リソース管理システムの実装
   - シーンマネージャーの実装
   - 物理ベースレンダリング（PBR）の導入

## Git コミット履歴とマイルストーン

1. [コア機能] プロジェクト基盤の実装 (Window, Application クラス)
2. [修正] GLAD初期化関数の更新
3. [ビルド] GLFWの静的ライブラリ利用への変更
4. [レンダリング] シェーダークラス実装と基本三角形描画
5. [ドキュメント] プロジェクトルールとドキュメントの改善
6. [レンダリング] 3Dモデル(.obj)の読み込みと表示、回転機能実装

## プラットフォーム別の準備事項

### Windows
1. Visual Studio 2019/2022をインストール（C++デスクトップ開発ワークロードを選択）
2. CMakeをインストール（パスを通す）
3. 必要に応じてDLLをビルドディレクトリに配置（CMakeの設定で自動コピーされます）

### Linux
以下のパッケージをインストールします：
```bash
# Debian/Ubuntu
sudo apt-get install build-essential cmake libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev

# Fedora/RHEL
sudo dnf install cmake gcc-c++ mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel
```

### macOS
1. Xcode Command Line Toolsをインストール
```bash
xcode-select --install
```
2. Homebrewを使ってCMakeをインストール（任意）
```bash
brew install cmake
```

## 実装上の注意点と一時的な制約

- **レンダリングコードの配置**: 現段階ではApplicationクラスにレンダリングコードを含めていますが、将来的にレンダリング専用クラスに分離します。
- **リソース管理**: 現時点では基本的なOpenGLリソース管理のみで、将来的により高度なリソースキャッシングや管理を実装予定です。
- **エラー処理**: シェーダーコンパイル時のエラー処理は充実していますが、機能拡張に合わせてより細かいエラーチェックを実装します。
- **マルチプラットフォーム対応**: 各プラットフォーム用のライブラリファイルをそれぞれ準備する必要があります。特にWindows/Linux用のGLFWライブラリは実行前に取得して配置する必要があります。
