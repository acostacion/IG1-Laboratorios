#include "Material.h"
#include "Shader.h"

Material::Material(glm::vec3 color, float shininess) {
    ambient = color * 0.2f;
    diffuse = color;
    specular = glm::vec3(0.5f);
    expF = shininess;
}

void Material::upload(Shader& shader) const {
    shader.setUniform("material.ambient", ambient);
    shader.setUniform("material.diffuse", diffuse);
    shader.setUniform("material.specular", specular);
    shader.setUniform("material.shininess", expF);
}

void Material::setCopper() {
    ambient = { 0.19125f,  0.0735f,   0.0225f };
    diffuse = { 0.7038f,   0.27048f,  0.0828f };
    specular = { 0.256777f, 0.137622f, 0.086014f };
    expF = 12.8f;
}

void Material::setGold() {
    ambient = { 0.24725f,  0.1995f,   0.0745f };
    diffuse = { 0.75164f,  0.60648f,  0.22648f };
    specular = { 0.628281f, 0.555802f, 0.366065f };
    expF = 51.2f;
}