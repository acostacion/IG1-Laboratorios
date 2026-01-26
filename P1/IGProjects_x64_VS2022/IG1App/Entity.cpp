#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Abs_Entity::upload(const mat4& modelViewMat) const {
	mShader->setUniform("modelView", modelViewMat);
}

Abs_Entity::~Abs_Entity() {
	delete mMesh;
	mMesh = nullptr;
}

void
Abs_Entity::load() {
	mMesh->load();
}

void
Abs_Entity::unload() {
	mMesh->unload();
}

EntityWithColors::EntityWithColors() {
	mShader = Shader::get("vcolors");
}

void
EntityWithColors::render(mat4 const& modelViewMat) const {
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
	}
}

SingleColorEntity::SingleColorEntity(glm::vec4 color) : mColor(color) {
	mShader = Shader::get("simple");
}

void SingleColorEntity::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("color", mColor);
		upload(aMat);
		mMesh->render();
	}
}

RGBAxes::RGBAxes(GLdouble l) {
	mMesh = Mesh::createRGBAxes(l);
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r) : SingleColorEntity() {
	mMesh = Mesh::generateRegularPolygon(num, r);
}
