#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "mesh.h"

namespace claude_gl {

/**
 * @brief 3Dモデルを管理するクラス
 * 
 * OBJファイルを読み込み、メッシュデータとして管理します
 */
class Model {
public:
    /**
     * @brief コンストラクタ
     * @param filepath OBJファイルのパス
     */
    explicit Model(const std::string& filepath);
    
    /**
     * @brief デストラクタ
     */
    ~Model() = default;
    
    /**
     * @brief モデルを描画
     * @param shader 使用するシェーダー
     */
    void draw(const Shader& shader) const;
    
    /**
     * @brief モデル行列の設定
     * @param model モデル変換行列
     */
    void setModelMatrix(const glm::mat4& model);
    
    /**
     * @brief モデル行列の取得
     * @return モデル変換行列
     */
    const glm::mat4& getModelMatrix() const;
    
    /**
     * @brief モデルのX軸回転
     * @param angle 回転角度（ラジアン）
     */
    void rotateX(float angle);
    
    /**
     * @brief モデルのY軸回転
     * @param angle 回転角度（ラジアン）
     */
    void rotateY(float angle);
    
    /**
     * @brief モデルのZ軸回転
     * @param angle 回転角度（ラジアン）
     */
    void rotateZ(float angle);
    
private:
    std::vector<std::shared_ptr<Mesh>> meshes;  ///< モデルを構成するメッシュ
    glm::mat4 modelMatrix;                      ///< モデル変換行列
    
    /**
     * @brief OBJファイルを読み込む
     * @param filepath OBJファイルのパス
     */
    void loadModel(const std::string& filepath);
    
    /**
     * @brief OBJファイルからメッシュを処理
     * @param filepath OBJファイルのパス
     */
    void processObjFile(const std::string& filepath);
};

} // namespace claude_gl