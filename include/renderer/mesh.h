#pragma once

#include <vector>
#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "shader.h"

namespace claude_gl {

/**
 * @brief 3Dメッシュを管理するクラス
 * 
 * 頂点データとインデックスデータを保持し、OpenGLによる描画を処理します
 */
class Mesh {
public:
    /**
     * @brief 頂点構造体
     */
    struct Vertex {
        glm::vec3 position;  ///< 頂点位置
        glm::vec3 normal;    ///< 法線ベクトル
        glm::vec2 texCoords; ///< テクスチャ座標
    };

    /**
     * @brief コンストラクタ
     * @param vertices 頂点データ
     * @param indices インデックスデータ
     */
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    
    /**
     * @brief デストラクタ
     */
    ~Mesh();
    
    /**
     * @brief メッシュを描画
     * @param shader 使用するシェーダー
     */
    void draw(const Shader& shader) const;
    
private:
    // メッシュデータ
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // OpenGLオブジェクト
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    
    /**
     * @brief OpenGLバッファの設定
     */
    void setupMesh();
};

} // namespace claude_gl