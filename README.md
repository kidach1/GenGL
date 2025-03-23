# Claude-OpenGL

モダンなOpenGLを使用した3Dグラフィックスアプリケーションのフレームワーク。
GLFW, GLAD, GLMを使用して、クロスプラットフォームなグラフィックスアプリケーションを構築するためのベースコードを提供します。

## 機能

- モダンなOpenGL 3.3+ コアプロファイルの使用
- GLFW によるクロスプラットフォームなウィンドウ管理
- シェーダープログラムの読み込みと管理
- 3Dメッシュの描画とモデルローディング（OBJフォーマット対応）
- レンダリングパイプラインの抽象化
- Windows、Linux、macOSのマルチプラットフォーム対応

## 要件

- C++17対応コンパイラ
- CMake 3.10以上
- OpenGL 3.3+対応グラフィックスカード/ドライバ

## プラットフォーム別の準備事項

### Windows
1. Visual Studio 2019/2022をインストール（C++デスクトップ開発ワークロードを選択）
2. CMakeをインストール（パスを通す）

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

## ビルド方法

### Windows
```bat
REM コマンドプロンプトから
build_windows.bat
```

または、Visual Studioで直接CMakeプロジェクトを開いてビルドすることも可能です。

### Linux
```bash
# ビルドスクリプトに実行権限を付与
chmod +x build_linux.sh

# ビルドスクリプト実行
./build_linux.sh
```

### macOS
```bash
# ビルドディレクトリの作成
mkdir -p build
cd build

# CMakeの実行
cmake ..

# ビルド
make

# ルートディレクトリに戻る
cd ..
```

## 実行方法

### Windows
```bat
REM buildディレクトリから
Release\Claude-OpenGL.exe
```

### Linux/macOS
```bash
# buildディレクトリから
./Claude-OpenGL
```

## プロジェクト構造

```
Claude-OpenGL/
├── src/                      # ソースコード
│   ├── main.cpp              # メインエントリーポイント
│   ├── core/                 # コア機能
│   ├── renderer/             # レンダリング関連コード
│   └── utils/                # ユーティリティ
├── include/                  # 公開ヘッダーファイル
├── assets/                   # アセット（シェーダー、テクスチャなど）
│   ├── models/               # 3Dモデルファイル
│   ├── shaders/              # シェーダープログラム
│   └── textures/             # テクスチャファイル
└── libs/                     # 外部ライブラリ
    ├── glad-3.3/             # GLAD 3.3
    ├── glfw/                 # GLFW
    └── glm/                  # GLM
```

## ライセンス

MITライセンスの下で公開されています。詳細はLICENSEファイルを参照してください。
