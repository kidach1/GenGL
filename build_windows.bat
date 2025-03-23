@echo off
REM Windows用のビルドスクリプト

REM ビルドディレクトリの作成
if not exist build mkdir build
cd build

REM CMakeの実行
cmake -G "Visual Studio 17 2022" -A x64 ..

echo.
echo ビルド設定が完了しました。
echo Visual Studioでプロジェクトを開くには:
echo   start Claude-OpenGL.sln
echo.
echo コマンドラインでビルドするには:
echo   cmake --build . --config Release
echo.

REM ビルドするか確認
set /p BUILD_NOW="ビルドを実行しますか？ (y/n): "
if /i "%BUILD_NOW%"=="y" (
    cmake --build . --config Release
    echo.
    echo ビルドが完了しました。
    echo 実行ファイルは build\Release\ にあります。
)

cd ..
