#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

namespace claude_gl {

Shader::Shader() : programId(0) {
}

Shader::~Shader() {
    // シェーダープログラムの削除
    if (programId != 0) {
        glDeleteProgram(programId);
        programId = 0;
    }
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    // ファイルの例外を有効化
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        // ファイルを開く
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        
        // ファイルバッファの内容をstreamに読み込む
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        // ファイルを閉じる
        vShaderFile.close();
        fShaderFile.close();
        
        // streamをstringに変換
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return false;
    }
    
    return loadFromString(vertexCode, fragmentCode);
}

bool Shader::loadFromString(const std::string& vertexSource, const std::string& fragmentSource) {
    // 以前のシェーダープログラムがあれば削除
    if (programId != 0) {
        glDeleteProgram(programId);
        uniformLocationCache.clear();
    }
    
    unsigned int vertexShader, fragmentShader;
    
    // 頂点シェーダーのコンパイル
    if (!compileShader(vertexShader, GL_VERTEX_SHADER, vertexSource)) {
        return false;
    }
    
    // フラグメントシェーダーのコンパイル
    if (!compileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentSource)) {
        glDeleteShader(vertexShader);
        return false;
    }
    
    // シェーダープログラムのリンク
    if (!linkProgram(vertexShader, fragmentShader)) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    // シェーダーオブジェクトはリンク後に削除可能
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

void Shader::use() const {
    if (programId != 0) {
        glUseProgram(programId);
    }
}

void Shader::unuse() const {
    glUseProgram(0);
}

unsigned int Shader::getId() const {
    return programId;
}

bool Shader::compileShader(unsigned int& shader, GLenum type, const std::string& source) {
    shader = glCreateShader(type);
    const char* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, NULL);
    glCompileShader(shader);
    
    // コンパイルエラーのチェック
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
        return false;
    }
    
    return true;
}

bool Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);
    
    // リンクエラーのチェック
    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        checkCompileErrors(programId, "PROGRAM");
        return false;
    }
    
    return true;
}

void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

int Shader::getUniformLocation(const std::string& name) const {
    // キャッシュ内に位置があるか確認
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end()) {
        return it->second;
    }
    
    // キャッシュになければ取得してキャッシュに保存
    int location = glGetUniformLocation(programId, name.c_str());
    uniformLocationCache[name] = location;
    
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' not found in shader program" << std::endl;
    }
    
    return location;
}

// Uniform設定メソッドの実装
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace claude_gl