#pragma once

#include <string>
#include <unordered_map>
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace claude_gl {

/**
 * @brief OpenGLシェーダープログラムを管理するクラス
 * 
 * シェーダーのロード、コンパイル、リンク、使用を担当し、
 * uniformパラメータの設定も行う
 */
class Shader {
public:
    /**
     * @brief シェーダーのデフォルトコンストラクタ
     */
    Shader();
    
    /**
     * @brief シェーダーのデストラクタ
     * 
     * OpenGLリソースを解放する
     */
    ~Shader();
    
    /**
     * @brief ファイルから頂点シェーダーとフラグメントシェーダーをロードする
     * 
     * @param vertexPath 頂点シェーダーのファイルパス
     * @param fragmentPath フラグメントシェーダーのファイルパス
     * @return 成功した場合はtrue、失敗した場合はfalse
     */
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    
    /**
     * @brief 文字列から頂点シェーダーとフラグメントシェーダーをロードする
     * 
     * @param vertexSource 頂点シェーダーのソースコード
     * @param fragmentSource フラグメントシェーダーのソースコード
     * @return 成功した場合はtrue、失敗した場合はfalse
     */
    bool loadFromString(const std::string& vertexSource, const std::string& fragmentSource);
    
    /**
     * @brief シェーダープログラムを使用する
     */
    void use() const;
    
    /**
     * @brief シェーダープログラムの使用を停止する
     */
    void unuse() const;
    
    /**
     * @brief bool型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setBool(const std::string& name, bool value) const;
    
    /**
     * @brief int型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setInt(const std::string& name, int value) const;
    
    /**
     * @brief float型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setFloat(const std::string& name, float value) const;
    
    /**
     * @brief vec2型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setVec2(const std::string& name, const glm::vec2& value) const;
    
    /**
     * @brief vec3型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setVec3(const std::string& name, const glm::vec3& value) const;
    
    /**
     * @brief vec4型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setVec4(const std::string& name, const glm::vec4& value) const;
    
    /**
     * @brief mat2型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setMat2(const std::string& name, const glm::mat2& value) const;
    
    /**
     * @brief mat3型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setMat3(const std::string& name, const glm::mat3& value) const;
    
    /**
     * @brief mat4型のuniform変数を設定する
     * 
     * @param name uniform変数の名前
     * @param value 設定する値
     */
    void setMat4(const std::string& name, const glm::mat4& value) const;
    
    /**
     * @brief シェーダープログラムIDを取得する
     * 
     * @return シェーダープログラムID
     */
    unsigned int getId() const;
    
private:
    // シェーダープログラムID
    unsigned int programId;
    
    // uniform位置のキャッシュ（パフォーマンス向上のため）
    mutable std::unordered_map<std::string, int> uniformLocationCache;
    
    /**
     * @brief シェーダーをコンパイルする
     * 
     * @param shader 作成されたシェーダーIDの格納先
     * @param type シェーダーの種類（GL_VERTEX_SHADER, GL_FRAGMENT_SHADERなど）
     * @param source シェーダーのソースコード
     * @return 成功した場合はtrue、失敗した場合はfalse
     */
    bool compileShader(unsigned int& shader, GLenum type, const std::string& source);
    
    /**
     * @brief シェーダープログラムをリンクする
     * 
     * @param vertexShader 頂点シェーダーID
     * @param fragmentShader フラグメントシェーダーID
     * @return 成功した場合はtrue、失敗した場合はfalse
     */
    bool linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    
    /**
     * @brief シェーダーのコンパイルエラーやリンクエラーをチェックする
     * 
     * @param shader シェーダーまたはプログラムID
     * @param type "VERTEX", "FRAGMENT", "PROGRAM"のいずれか
     */
    void checkCompileErrors(unsigned int shader, const std::string& type);
    
    /**
     * @brief uniform変数の位置を取得する（キャッシング付き）
     * 
     * @param name uniform変数の名前
     * @return uniform変数の位置
     */
    int getUniformLocation(const std::string& name) const;
};

} // namespace claude_gl