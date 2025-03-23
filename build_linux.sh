#!/bin/bash
# Linux用のビルドスクリプト

# ビルドディレクトリの作成
mkdir -p build
cd build

# CMakeの実行
cmake ..

echo
echo "ビルド設定が完了しました。"
echo "ビルドを実行するには: make"
echo

# ビルドするか確認
read -p "ビルドを実行しますか？ (y/n): " BUILD_NOW
if [[ $BUILD_NOW == "y" || $BUILD_NOW == "Y" ]]; then
    make
    echo
    echo "ビルドが完了しました。"
    echo "実行ファイルは build/ ディレクトリにあります。"
fi

cd ..
