#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// 出力値
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// 変換行列
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // 頂点位置を計算
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // フラグメントシェーダーに渡す値
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // 法線変換（非均一スケーリング対応）
    TexCoords = aTexCoords;
}