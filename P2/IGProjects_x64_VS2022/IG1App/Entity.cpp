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
	
	if (_angle >= 180)
	{ // Cuando llegue a 180 (en la animacion) se reinicia el angulo y se pasa al siguiente estado de animacion.
		_angle = 0;
		_axisState++;
	}

	// Cuando se complete la animacion se reinicia el estado y vuelta a empezar.
	if (_axisState == 3) _axisState = 0;

	_angle++; // va iterando el angle
	
}

void RGBCube::rotateOnAxis(GLint n) {
	switch (n)
	{
	case 0: // x
		mModelMat = rotate(dmat4(1), radians<double>(_angle), dvec3(1, 0, 0))
			* translate(dmat4(1), dvec3(_l / 2, _l / 2, -_l / 2));
		break;

	case 1: // z
		mModelMat = rotate(dmat4(1), radians<double>(_angle), dvec3(0, 0, 1))
			* translate(dmat4(1), dvec3(_l / 2, -_l / 2, _l / 2));
		break;

	case 2: // y
		mModelMat = rotate(dmat4(1), radians<double>(_angle), dvec3(0, 1, 0))
			* translate(dmat4(1), dvec3(-_l / 2, _l / 2, _l / 2));
		break;

	default:break;
	}
}

RGBTriangle::RGBTriangle(GLdouble l, GLdouble r) {
	mMesh = Mesh::generateRegularPolygon(3, l);
	mMesh->setPrimitive(GL_TRIANGLES);
	// se pone a cada vértice un color diferente
	mMesh->setColorBuffer({
	  {1.0f, 0.0f, 0.0f, 1.0f}, // rojo
	  {0.0f, 1.0f, 0.0f, 1.0f}, // verde
	  {0.0f, 0.0f, 1.0f, 1.0f}  // azul
		});
	mModelMat = translate(glm::dmat4(1), glm::dvec3(r, 0.0, 0.0));
}

void RGBTriangle::update()
{
	float delta = glm::radians(5.0f); // 5 grados por update
	mModelMat = glm::rotate(mModelMat, -delta*2, glm::vec3(0, 0, 1));
	mModelMat = glm::rotate(glm::mat4(1.0f), delta, glm::vec3(0, 0, 1)) * mModelMat;
}

RGBRectangle::RGBRectangle(GLdouble w, GLdouble h) {
	mMesh = Mesh::generateRGBRectangle(w, h);
}

void RGBRectangle::render(const mat4& modelViewMat) const {
	if (mMesh != nullptr) {

		glEnable(GL_CULL_FACE);
		// CARA DE DELANTE
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
		// CARA DE ATRAS
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();

		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	}
}

BoxOutline::BoxOutline(GLdouble length) 
	: EntityWithTexture() {
	mMesh = Mesh::generateBoxOutlineTexCor(length);
}

void BoxOutline::render(const glm::mat4& modelViewMat) const { // TODO luego cambiar a la version de texturas
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		if (mTexture != nullptr) // si la textura no es nula podemos proceder a renderizarla
		{
			glEnable(GL_CULL_FACE);
			// CARA DE DELANTE
			mTexture->bind();
			glCullFace(GL_BACK);
			mMesh->render();
			mTexture->unbind();

			// CARA DE ATRAS
			mTextureInterior->bind();
			glCullFace(GL_FRONT);
			mMesh->render();
			mTextureInterior->unbind();
			glDisable(GL_CULL_FACE);
		}
	}
}

Ground::Ground(GLdouble w, GLdouble h) {
	mMesh = Mesh::generateRectangleTexCor(w, h, 4, 4); // rw=rh=4 como en el ejemplo
	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\baldosaC.png"); // ajusta la ruta a tu proyecto
	mModelMat = glm::rotate(glm::dmat4(1), glm::radians(-90.0), glm::dvec3(1, 0, 0));
}

EntityWithTexture::EntityWithTexture() {
	mShader = Shader::get("texture");
}

void EntityWithTexture::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		glm::mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);
		if (mTexture != nullptr) mTexture->bind();
		mMesh->render();
		if (mTexture != nullptr) mTexture->unbind();
	}
}

Star3D::Star3D(GLdouble re, GLuint np, GLdouble h)
	: EntityWithTexture(), _h(h) {
	mMesh = Mesh::generateStar3DTexCor(re, np, h);
}

void Star3D::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mShader->use();
		mShader->setUniform("modulate", mModulate);

		if (mTexture != nullptr) mTexture->bind();

		// Primera estrella
		glm::mat4 aMat = modelViewMat * mModelMat;
		upload(aMat);
		mMesh->render();

		// Segunda estrella volteada
		glm::mat4 aMatFlip = aMat * glm::rotate(glm::mat4(1.0f),
			glm::radians(180.0f), glm::vec3(1, 0, 0));
		upload(aMatFlip);
		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind();
	}
}

void Star3D::update() {
	_zAngle += 2.0f; // velocidad giro sobre Z
	_yAngle += 1.0f; // velocidad giro sobre Y (más lento)

	mModelMat = glm::rotate(glm::mat4(1.0f), glm::radians(_yAngle), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(_zAngle), glm::vec3(0, 0, 1));
}