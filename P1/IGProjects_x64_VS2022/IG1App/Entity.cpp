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

void Abs_Entity::update() {
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

Cube::Cube(GLdouble l) : SingleColorEntity(glm::vec4(0.0f)) {
	mMesh = Mesh::generateCube(l);
}

void Cube::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("color", mColor);
		upload(aMat);

		glEnable(GL_CULL_FACE);
			// CARA DE DELANTE
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			mMesh->render();

			// CARA DE ATRAS
			glCullFace(GL_FRONT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			mMesh->render();
		glDisable(GL_CULL_FACE);
	}
}

RGBCube::RGBCube(GLdouble l) : EntityWithColors(), _l(l) {
	mMesh = Mesh::generateRGBCubeTriangles(_l);
}

void RGBCube::update() {
	// hace la animacion en X inicialmente.
	rotateOnAxis(_axisState);

	// ¡¡¡OJO!!! como en rotateOnAxis hacemos angle/2 para que sea 180, aquí lo tenemos que hacer 360.
	if (angle >= 360)
	{ // Cuando llegue a 180 (en la animacion) se reinicia el angulo y se pasa al siguiente estado de animacion.
		angle = 0;
		_axisState++;
	}

	// Cuando se complete la animacion se reinicia el estado y vuelta a empezar.
	if (_axisState == 3) _axisState = 0;

	angle++; // va iterando el angle
}

void RGBCube::rotateOnAxis(GLint n) {
	//switch (n)
	//{
	//case 0: // x
	//	mModelMat = rotate(glm::dmat4(1), radians(angle / 2), glm::dvec3(1, 0, 0))
	//		* translate(glm::dmat4(1), glm::dvec3(_l / 2, _l / 2, -_l / 2));
	//	break;

	//case 1: // z
	//	mModelMat = rotate(glm::dmat4(1), radians(angle / 2), glm::dvec3(0, 0, 1))
	//		* translate(glm::dmat4(1), glm::dvec3(_l / 2, -_l / 2, _l / 2));
	//	break;

	//case 2: // y
	//	mModelMat = rotate(glm::dmat4(1), radians(angle / 2), glm::dvec3(0, 1, 0))
	//		* translate(glm::dmat4(1), glm::dvec3(-_l / 2, _l / 2, _l / 2));
	//	break;

	//default:break;
	//}
}

RGBTriangle::RGBTriangle(GLdouble l) {
	mMesh = Mesh::generateRegularPolygon(3, l);
	mMesh->setPrimitive(GL_TRIANGLES);
	// se pone a cada vértice un color diferente
	mMesh->setColorBuffer({
	  {1.0f, 0.0f, 0.0f, 1.0f}, // rojo
	  {0.0f, 1.0f, 0.0f, 1.0f}, // verde
	  {0.0f, 0.0f, 1.0f, 1.0f}  // azul
		});
}

RGBRectangle::RGBRectangle(GLdouble w, GLdouble h) {
	mMesh = Mesh::generateRGBRectangle(w, h);
}

void RGBRectangle::render(const mat4& modelViewMat) const {
	if (mMesh != nullptr) {

		glEnable(GL_CULL_FACE);
		// CARA DE DELANTE
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
		// CARA DE ATRAS
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();

		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	}
}


