#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// ライト情報
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

// マテリアル情報
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform int shininess;

void main() {
    // 環境光（アンビエント）
    vec3 ambient = ambientStrength * lightColor;
    
    // 拡散光（ディフューズ）- 法線の視覚化も追加
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // 法線を視覚化するためのカラー (法線の方向をRGBカラーとして表現)
    vec3 normalColor = norm * 0.5 + 0.5; // [-1,1] から [0,1] の範囲に変換
    
    // 鏡面反射光（スペキュラー）
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    
    // 合成 (50%法線可視化、50%ライティング)
    vec3 lighting = (ambient + diffuse + specular) * objectColor;
    vec3 result = mix(lighting, normalColor, 0.5); // 法線可視化とライティングを50%ずつ混合
    
    FragColor = vec4(result, 1.0);
}