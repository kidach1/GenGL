#pragma once

#include <string>
#include <filesystem>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#else
    #include <unistd.h>
    #include <linux/limits.h>
#endif

namespace claude_gl {
namespace utils {

/**
 * @brief プラットフォーム固有の処理を扱うユーティリティクラス
 */
class PlatformUtils {
public:
    /**
     * @brief 実行可能ファイルのディレクトリパスを取得
     * @return 実行可能ファイルのディレクトリパス
     */
    static std::string getExecutablePath() {
#if defined(_WIN32)
        // Windows実装
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        std::string execPath(path);
        size_t lastSlash = execPath.find_last_of("\\/");
        return execPath.substr(0, lastSlash);
#elif defined(__APPLE__)
        // macOS実装
        char path[1024];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0) {
            std::string execPath(path);
            size_t lastSlash = execPath.find_last_of("/");
            return execPath.substr(0, lastSlash);
        }
        return "./";
#else
        // Linux実装
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            std::string execPath(result, count);
            size_t lastSlash = execPath.find_last_of("/");
            return execPath.substr(0, lastSlash);
        }
        return "./";
#endif
    }

    /**
     * @brief プラットフォーム依存のパスセパレータを使用してパスを結合
     * @param base ベースパス
     * @param append 追加するパス
     * @return 結合されたパス
     */
    static std::string joinPath(const std::string& base, const std::string& append) {
        // C++17のfilesystemを使用してプラットフォーム間の互換性を確保
        std::filesystem::path basePath(base);
        std::filesystem::path appendPath(append);
        std::filesystem::path fullPath = basePath / appendPath;
        return fullPath.string();
    }
    
    /**
     * @brief テクスチャパスをプラットフォーム依存パスに変換
     * @param relativePath アセットへの相対パス
     * @return 適切に変換されたパス
     */
    static std::string getAssetPath(const std::string& relativePath) {
        // プロジェクト基準の相対パスを実行ファイル基準のパスに変換
        return joinPath(getExecutablePath(), relativePath);
    }
};

} // namespace utils
} // namespace claude_gl