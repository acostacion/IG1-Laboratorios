#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

using namespace glm;

Material::Material(vec3 color, float shininess)
 : ambient(color)
 , diffuse(color)
 , specular(color)
 , expF(shininess)
{
}

void
Material::upload(Shader& lighting) const
{
	lighting.setUniform("material.ambient", ambient);
	lighting.setUniform("material.diffuse", diffuse);
	lighting.setUniform("material.specular", specular);
	lighting.setUniform("material.shininess", expF);
}

void
Material::setCopper()
{
	ambient = {0.19125, 0.0735, 0.0225};
	diffuse = {0.7038, 0.27048, 0.0828};
	specular = {0.256777, 0.137622, 0.086014};
	expF = 12.8;
}

void
Material::setGold()
{
	ambient = { 0.24725, 0.1995, 0.0745 };
	diffuse = { 0.75164, 0.60648, 0.22648 };
	specular = { 0.628281, 0.555802, 0.366065 };
	expF = 51.2;
}
