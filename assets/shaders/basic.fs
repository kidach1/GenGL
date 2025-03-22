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
    
    // 拡散光（ディフューズ）
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // 鏡面反射光（スペキュラー）
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    
    // 合成
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}