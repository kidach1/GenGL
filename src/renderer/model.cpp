#include "renderer/model.h"
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
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            
            std::array<std::string, 3> vertexData = { v1, v2, v3 };
            for (const auto& vertex : vertexData) {
                // 頂点情報を「/」で分割
                std::vector<std::string> tokens;
                std::istringstream tokenStream(vertex);
                std::string token;
                
                while (std::getline(tokenStream, token, '/')) {
                    tokens.push_back(token);
                }
                
                // OBJファイルのフォーマットに応じて処理
                // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                unsigned int posIndex = 0, texIndex = 0, normIndex = 0;
                
                if (tokens.size() >= 1 && !tokens[0].empty()) {
                    posIndex = std::stoi(tokens[0]) - 1; // OBJファイルは1から始まるインデックス
                }
                
                if (tokens.size() >= 2 && !tokens[1].empty()) {
                    texIndex = std::stoi(tokens[1]) - 1;
                }
                
                if (tokens.size() >= 3 && !tokens[2].empty()) {
                    normIndex = std::stoi(tokens[2]) - 1;
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
        std::cout << "Loaded mesh with " << vertices.size() << " vertices and " 
                  << indices.size() << " indices from " << filepath << std::endl;
        std::cout << "Normals count in OBJ: " << normals.size() << std::endl;
    }
    else {
        std::cerr << "Warning: No geometry data loaded from " << filepath << std::endl;
    }
}

} // namespace claude_gl