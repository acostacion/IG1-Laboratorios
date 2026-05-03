#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Shader; // forward declaration

class Material {
public:
    Material() = default;
    virtual ~Material() = default;

    // Constructor con color uniforme y shininess
    Material(glm::vec3 color, float shininess = 8.0f);

    // Sube los atributos al shader light
    void upload(Shader& shader) const;

    // Setters
    void setAmbient(const glm::vec3& a) { ambient = a; }
    void setDiffuse(const glm::vec3& d) { diffuse = d; }
    void setSpecular(const glm::vec3& s) { specular = s; }
    void setShininess(GLfloat s) { expF = s; }

    // Materiales predefinidos
    void setCopper();
    void setGold();

protected:
    glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
    glm::vec3 diffuse = { 0.8f, 0.8f, 0.8f };
    glm::vec3 specular = { 0.5f, 0.5f, 0.5f };
    GLfloat   expF = 8.0f;
};