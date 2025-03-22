#include "renderer/mesh.h"
#include <iostream>

namespace claude_gl {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices), vao(0), vbo(0), ebo(0) {
    setupMesh();
}

Mesh::~Mesh() {
    // OpenGLリソースの解放
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
    if (ebo != 0) {
        glDeleteBuffers(1, &ebo);
    }
}

void Mesh::setupMesh() {
    // OpenGLバッファの生成
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // VAOのバインド
    glBindVertexArray(vao);
    
    // VBOの設定
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    // EBOの設定
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    // 頂点属性の設定
    // 位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // 法線属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    // テクスチャ座標属性
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    
    // VAOのバインド解除
    glBindVertexArray(0);
}

void Mesh::draw(const Shader& shader) const {
    // VAOをバインドして描画
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace claude_gl