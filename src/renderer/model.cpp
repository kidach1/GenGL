#include "../../include/renderer/model.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace claude_gl {

Model::Model(const std::string& filepath)
    : modelMatrix(1.0f) {
    loadModel(filepath);
}

void Model::draw(const Shader& shader) const {
    // モデル行列をシェーダーに設定
    shader.setMat4("model", modelMatrix);
    
    // 全てのメッシュを描画
    for (const auto& mesh : meshes) {
        mesh->draw(shader);
    }
}

void Model::setModelMatrix(const glm::mat4& model) {
    modelMatrix = model;
}

const glm::mat4& Model::getModelMatrix() const {
    return modelMatrix;
}

void Model::rotateX(float angle) {
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Model::rotateY(float angle) {
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Model::rotateZ(float angle) {
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::loadModel(const std::string& filepath) {
    try {
        processObjFile(filepath);
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading model " << filepath << ": " << e.what() << std::endl;
    }
}

void Model::processObjFile(const std::string& filepath) {
    // OBJファイルを読み込むための一時データ構造
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // OBJファイルを開く
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    
    std::string line;
    unsigned int index = 0;
    
    // ファイルを1行ずつ読み込み
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "v") {
            // 頂点位置
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (prefix == "vn") {
            // 頂点法線
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "vt") {
            // テクスチャ座標
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        }
        else if (prefix == "f") {
            // 面情報（頂点インデックス）
            std::string vertex;
            
            // OBJファイルのf行は次のフォーマットのいずれかで記述されている：
            // f v1 v2 v3 ... （位置のみ）
            // f v1/vt1 v2/vt2 v3/vt3 ... （位置とテクスチャ座標）
            // f v1//vn1 v2//vn2 v3//vn3 ... （位置と法線）
            // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ... （位置、テクスチャ座標、法線）
            
            // 簡易実装: まず3つの頂点を処理（三角形の各頂点）
            for (int i = 0; i < 3; i++) {
                iss >> vertex;
                
                // 頂点情報を「/」で分割
                std::replace(vertex.begin(), vertex.end(), '/', ' ');
                std::istringstream viss(vertex);
                
                unsigned int posIndex = 0, texIndex = 0, normIndex = 0;
                
                // 位置インデックスは必須
                viss >> posIndex;
                posIndex--; // OBJファイルは1から始まるインデックスだが、C++は0から
                
                // テクスチャ座標インデックス（存在する場合）
                if (viss >> texIndex) {
                    texIndex--;
                }
                
                // 法線インデックス（存在する場合）
                if (viss >> normIndex) {
                    normIndex--;
                }
                
                Mesh::Vertex meshVertex;
                
                // インデックスが有効範囲内かチェック
                if (posIndex < positions.size()) {
                    meshVertex.position = positions[posIndex];
                }
                
                if (texIndex < texCoords.size()) {
                    meshVertex.texCoords = texCoords[texIndex];
                }
                else {
                    // テクスチャ座標がない場合はデフォルト値
                    meshVertex.texCoords = glm::vec2(0.0f, 0.0f);
                }
                
                if (normIndex < normals.size()) {
                    meshVertex.normal = normals[normIndex];
                }
                else {
                    // 法線がない場合はデフォルト値
                    meshVertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);
                }
                
                vertices.push_back(meshVertex);
                indices.push_back(index++);
            }
        }
    }
    
    // 読み込んだデータからメッシュを作成
    if (!vertices.empty() && !indices.empty()) {
        meshes.push_back(std::make_shared<Mesh>(vertices, indices));
    }
    else {
        std::cerr << "Warning: No geometry data loaded from " << filepath << std::endl;
    }
}

} // namespace claude_gl