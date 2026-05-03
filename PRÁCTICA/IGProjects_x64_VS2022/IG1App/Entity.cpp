#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

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

EntityWithTexture::~EntityWithTexture() {
	delete mTexture; mTexture = nullptr;
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

	mModelMat = mBaseTransform                                                          // posición/escala base
		* glm::rotate(glm::mat4(1.0f), glm::radians(_yAngle), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(_zAngle), glm::vec3(0, 0, 1));
}


Photo::Photo(GLdouble w, GLdouble h) : EntityWithTexture() {
	mMesh = Mesh::generateRectangleTexCor(w, h);
	mModelMat = rotate(dmat4(1), radians(90.0), glm::dvec3(0, 1, 0)) * rotate(dmat4(1), radians(-90.0), glm::dvec3(1, 0, 0));
}

void Photo::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		if (mTexture != nullptr) // si la textura no es nula podemos proceder a renderizarla
		{
			mTexture->bind(); // activa la textura en la gpu

			// culling
			glEnable(GL_CULL_FACE);
			// CARA DE DELANTE
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mMesh->render();

			// CARA DE ATRAS
			glCullFace(GL_FRONT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mMesh->render();
			glDisable(GL_CULL_FACE);

			mTexture->unbind(); // activa la textura en la gpu
		}
	}
}

void Photo::update() {
	// actualiza la textura 
	mTexture->loadColorBuffer(800.0, 600.0);
}

GlassParapet::GlassParapet(GLdouble length) 
	: EntityWithTexture() {
	mMesh = Mesh::generateBoxOutlineTexCor(length);
}

void GlassParapet::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		if (mTexture != nullptr) // si la textura no es nula podemos proceder a renderizarla
		{
			mTexture->bind(); // activa la textura en la gpu

			// culling
			glEnable(GL_CULL_FACE);
			// CARA DE DELANTE
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mMesh->render();

			// CARA DE ATRAS
			glCullFace(GL_FRONT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mMesh->render();
			glDisable(GL_CULL_FACE);

			mTexture->unbind(); // activa la textura en la gpu
		}
	}
}

Box::Box(GLdouble length) 
	: EntityWithTexture(), _length(length) {
	mMesh = Mesh::generateBoxOutlineTexCor(_length);
	mMeshTapaAbj = Mesh::generateRectangleTexCor(_length, _length);
	mMeshTapaArr = Mesh::generateRectangleTexCor(_length, _length);
}

Box::~Box() {
	delete mMesh;
	mMesh = nullptr;

	delete mMeshTapaAbj;
	mMeshTapaAbj = nullptr;

	delete mMeshTapaArr;
	mMeshTapaArr = nullptr;
}

void Box::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr && mTexture != nullptr)
	{
		// --- Caja Principal 
		renderBox(modelViewMat);

		// --- Tapa abajo 
		renderBoxLower(modelViewMat);

		// --- Tapa arriba 
		renderBoxUpper(modelViewMat);
	}
}

void Box::renderBox(const glm::mat4& modelViewMat) const {
	mat4 aMat = modelViewMat * mModelMat;
	mShader->use();
	mShader->setUniform("modulate", mModulate);
	upload(aMat);

	// culling
	glEnable(GL_CULL_FACE);
	// CARA DE DELANTE
	mTexture->bind(); // activa la textura en la gpu
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mMesh->render();
	mTexture->unbind(); // desactiva la textura en la gpu

	// CARA DE ATRAS
	mTextureInterior->bind(); // activa la textura en la gpu
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mMesh->render();
	mTextureInterior->unbind(); // desactiva la textura en la gpu
	glDisable(GL_CULL_FACE);
}

void Box::renderBoxUpper(const glm::mat4& modelViewMat) const {
	mat4 cMat = modelViewMat * mModelMatArr
		* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));
	upload(cMat);

	// culling
	glEnable(GL_CULL_FACE);
	// CARA DE DELANTE
	mTexture->bind(); // activa la textura en la gpu
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mMeshTapaArr->render();
	mTexture->unbind(); // desactiva la textura en la gpu

	// CARA DE ATRAS
	mTextureInterior->bind(); // activa la textura en la gpu
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mMeshTapaArr->render();
	mTextureInterior->unbind(); // desactiva la textura en la gpu
	glDisable(GL_CULL_FACE);
}

void Box::renderBoxLower(const glm::mat4& modelViewMat) const {
	mat4 bMat = modelViewMat * mModelMatAbj
		* glm::translate(glm::mat4(1), glm::vec3(0, -_length / 2.0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));
	upload(bMat);

	// culling
	glEnable(GL_CULL_FACE);
	// CARA DE DELANTE
	mTexture->bind(); // activa la textura en la gpu
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mMeshTapaAbj->render();
	mTexture->unbind(); // desactiva la textura en la gpu

	// CARA DE ATRAS
	mTextureInterior->bind(); // activa la textura en la gpu
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mMeshTapaAbj->render();
	mTextureInterior->unbind(); // desactiva la textura en la gpu
	glDisable(GL_CULL_FACE);
}

void Box::update() {

	mModelMatArr = mBaseTranslation                                              // posición base
		* translate(glm::dmat4(1), glm::dvec3(-_length / 2, _length / 2, 0))
		* rotate(dmat4(1), radians(angle), dvec3(0.0, 0.0, 1.0))
		* translate(glm::dmat4(1), glm::dvec3(_length / 2, 0, 0));

	// ---- gestion estado
	// cerrando && angulo <= 0 -> abriendo
	if (openState == 1.0 && angle <= 0.0) openState = 0.0;
	// abriendo && angulo >= 180 -> cerrando
	else if (openState == 0.0 && angle >= 180.0) openState = 1.0;

	// ---- gestion angulo
	// abriendo && angulo <= 180 -> angulo++ (abre)
	if (angle < 180 && openState == 0.0) angle++;
	// cerrando && angulo >= 0	 -> angulo-- (cierra)
	else if (angle > 0 && openState == 1.0) angle--;
}

void Box::load() {
	mMesh->load();
	mMeshTapaAbj->load();
	mMeshTapaArr->load();
}

void Box::unload() {
	mMesh->unload();
	mMeshTapaAbj->unload();
	mMeshTapaArr->unload();
}

Grass::Grass(GLdouble l) 
	: EntityWithTexture(), _length(l) {
	mMesh = Mesh::generateRectangleTexCor(l, l);
	mShader = Shader::get("texture:texture_alpha");
}

void Grass::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr && mTexture != nullptr) {
		mTexture->bind();	 // activa la textura en la gpu
		createFirstGrass(modelViewMat);
		createSecondGrass(modelViewMat);
		createThirdGrass(modelViewMat);
		mTexture->unbind();  // desactiva la textura en la gpu
	}
}

void Grass::createFirstGrass(const glm::mat4& modelViewMat) const {
	mat4 aMat = modelViewMat * mModelMat;

	mShader->use();
	mShader->setUniform("modulate", mModulate);
	upload(aMat);

	// culling
	glEnable(GL_CULL_FACE);
	// CARA DE DELANTE
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mMesh->render();

	// CARA DE ATRAS
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mMesh->render();
	glDisable(GL_CULL_FACE);
}

void Grass::createSecondGrass(const glm::mat4& modelViewMat) const {
	mat4 bMat = modelViewMat * mModelMat
		* glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0, 1, 0));

	mShader->use();
	mShader->setUniform("modulate", mModulate);
	upload(bMat);
	mMesh->render();
}

void Grass::createThirdGrass(const glm::mat4& modelViewMat) const {
	mat4 cMat = modelViewMat * mModelMat
		* glm::rotate(glm::mat4(1.0f), glm::radians(-60.0f), glm::vec3(0, 1, 0));

	mShader->use();
	mShader->setUniform("modulate", mModulate);
	upload(cMat);
	mMesh->render();
}

Torus::Torus(GLdouble R, GLdouble r, GLuint nPoints, GLuint nSamples) {
	mShader = Shader::get("simple_light");
	//mShader = Shader::get("normals");
	std::vector<glm::vec2> profile;

	// Se van guardando en sentido antihorario desde x = 0
	GLdouble alpha = 90.0;
	GLdouble incremento = 360.0 / nPoints;
	//Conseguimos los puntos del perfil
	for (GLuint i = 0; i < nPoints + 2; i++)
	{
		GLdouble x = r * glm::cos(glm::radians(alpha)) + R;
		GLdouble y = r * glm::sin(glm::radians(alpha)); // el radio en la y no porq si no se sube.
		alpha += incremento;

		profile.emplace_back(x, y);
	}

	//Hacemos la malla por revolucion
	mMesh = IndexMesh::generateByRevolution(profile, nSamples, 2 * std::numbers::pi);
	//setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}
/*
void Torus::render(const glm::mat4& modelViewMat) const {

	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("color", mColor);
		upload(aMat);

		mMesh->render();
	}
}
*/
ColorMaterialEntity::ColorMaterialEntity() {
	mShader = Shader::get("simple_light"); // TODO tiene q estar???
}

Sphere::Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians) {
	mMesh = IndexMesh::generateSphere(radius, nParallels, nMeridians);
}

Disk::Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples) {
	std::vector<glm::vec2> profile;

	//El perfil es una linea recta
	GLdouble incremento = (R - r) / nRings;
	//Conseguimos los puntos del perfil
	for (GLuint i = 0; i < nRings + 2; i++)
	{
		GLdouble x = r + (incremento * i);
		GLdouble y = 0;

		profile.emplace_back(x, y);
	}

	//Hacemos la malla por revolucion
	mMesh = IndexMesh::generateByRevolution(profile, nSamples, 2 * std::numbers::pi);

}

Cone::Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples) {
	std::vector<glm::vec2> profile;

	// zona de abajo
	GLdouble incremento = r / nRings;
	for (GLuint i = 0; i < nRings; i++)
	{
		GLdouble x = incremento * i;
		GLdouble y = -h / 2;

		profile.emplace_back(x, y);
	}

	// zona de arriba.
	incremento = R / nRings;
	for (GLuint i = nRings; i > 0; i--)
	{
		GLdouble x = incremento * (i - 1); // i-1 para cerrar.
		GLdouble y = h / 2;

		profile.emplace_back(x, y);
	}

	//Hacemos la malla por revolucion
	mMesh = IndexMesh::generateByRevolution(profile, nSamples, 2 * std::numbers::pi);
}

CompoundEntity::~CompoundEntity() {
	for (Abs_Entity* el : gObjects) delete el;
	gObjects.clear();
}

void CompoundEntity::render(const glm::mat4& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
	for (Abs_Entity* el : gObjects)
		el->render(aMat);
	
}

void CompoundEntity::update() {
	for (Abs_Entity* el : gObjects)
		el->update();
}

void CompoundEntity::load() {
	for (Abs_Entity* el : gObjects)
		el->load();
}

void CompoundEntity::unload() {
	for (Abs_Entity* el : gObjects)
		el->unload();
}

void CompoundEntity::addEntity(Abs_Entity* ae) {
	gObjects.emplace_back(ae);
}

SphereWithTexture::SphereWithTexture(GLdouble radius, GLuint nParallels, GLuint nMeridians) {
	mMesh = IndexMesh::generateSphere(radius, nParallels, nMeridians);
	mTexture = new Texture();
	mTexture->load("../assets/images/container.jpg", 255);
}

Droid::Droid(GLdouble radius) {
	addEntity(createHead(radius));

	Cone* eyeRight = createEye(radius);
	Cone* eyeLeft = createEye(radius);

	eyeRight->setModelMat(
		glm::translate(glm::dmat4(1), glm::dvec3(radius/16, 0.0, radius * 0.8))
		* glm::rotate(glm::dmat4(1), radians(90.0), glm::dvec3(1.0, 0.0, 0.0))
	);

	eyeLeft->setModelMat(
		glm::translate(glm::dmat4(1), glm::dvec3(-radius/16, 0.0, radius * 0.8))
		* glm::rotate(glm::dmat4(1), radians(90.0), glm::dvec3(1.0, 0.0, 0.0))
	);

	addEntity(eyeRight);
	addEntity(eyeLeft);
}

Cone* Droid::createHead(GLdouble r) {
	Cone* head = new Cone(r/2, r , r / 2, r, r);
	head->setMaterial(glm::vec3(1.0f, 1.0f, 0.0f)); // amarillo
	return head;
}

Cone* Droid::createEye(GLdouble r) {
	Cone* eye = new Cone(r, r/16, r/16, r, r);
	eye->setMaterial(glm::vec3(0.0f, 0.8f, 0.0f));
	return eye;
}

// TODO falla iluminacion
Robot::Robot(GLdouble radius) {
	addEntity(new SphereWithTexture(radius, 20, 20));

	Droid* head = new Droid(radius);
	head->setModelMat(glm::translate(glm::dmat4(1), glm::dvec3(0, radius, 0)));
	addEntity(head);
}

SnowManHat::SnowManHat(GLdouble radius) {
	Cone* innerC = new Cone(radius/2, radius - 1, radius - 1, 20, 20);
	Cone* outerC = new Cone((radius / 2) - 1, radius, radius, 20, 20);
	Cone* lowerC = new Cone(1, 3 * radius / 2, 3 * radius / 2, 20, 20);

	innerC->setMaterial(glm::vec3(1.0f, 0.0f, 0.0f));
	outerC->setMaterial(glm::vec3(0.0f, 0.0f, 1.0f));
	lowerC->setMaterial(glm::vec3(0.0f, 1.0f, 1.0f));

	innerC->setModelMat(glm::translate(glm::dmat4(1), glm::dvec3(0.0, radius / 4, 0.0)));
	outerC->setModelMat(glm::translate(glm::dmat4(1), glm::dvec3(0.0, radius / 4, 0.0)));

	addEntity(innerC);
	addEntity(outerC);
	addEntity(lowerC);
}

SnowManHead::SnowManHead(GLdouble radius) {
	Sphere* head = new Sphere(radius, 20, 20);
	head->setMaterial(glm::vec3(0.95f, 0.95f, 0.95f));
	addEntity(head);

	Cone* leftEye = createEye(radius);
	leftEye->setModelMat(
		glm::translate(glm::dmat4(1), glm::dvec3(radius / 3, 0.0, radius))
		* glm::rotate(glm::dmat4(1), radians(90.0), glm::dvec3(1.0, 0.0, 0.0))
	);
	Cone* rightEye = createEye(radius);
	rightEye->setModelMat(
		glm::translate(glm::dmat4(1), glm::dvec3(-radius / 3, 0.0, radius))
		* glm::rotate(glm::dmat4(1), radians(90.0), glm::dvec3(1.0, 0.0, 0.0))
	);

	addEntity(leftEye);
	addEntity(rightEye);

	Cone* nose = createNose(radius);
	nose->setModelMat(
		glm::translate(glm::dmat4(1), glm::dvec3(0.0, -radius/3, radius))
		* glm::rotate(glm::dmat4(1), radians(90.0), glm::dvec3(1.0, 0.0, 0.0))
	);
	addEntity(nose);

	SnowManHat* hat = new SnowManHat(radius);
	hat->setModelMat(glm::translate(glm::dmat4(1), glm::dvec3(0.0, radius/2, 0.0)));
	addEntity(hat);
}

Cone* SnowManHead::createEye(GLdouble r) {
	Cone* eye = new Cone(r / 3, r / 4, 0, 20, 20);
	eye->setMaterial(glm::vec3(0.0f, 0.5f, 0.5f));
	return eye;
}

Cone* SnowManHead::createNose(GLdouble r) {
	Cone* nose = new Cone(r , r / 8, 0, 20, 20);
	nose->setMaterial(glm::vec3(1.0f, 0.5f, 0.0f));
	return nose;
}

SnowMan::SnowMan(GLdouble radius) {
	Sphere* body = new Sphere(radius, 20, 20);
	body->setMaterial(glm::vec3(0.95f, 0.95f, 0.95f));
	addEntity(body);

	SnowManHead* head = new SnowManHead(2 * radius / 3);
	head->setModelMat(glm::translate(glm::dmat4(1), glm::dvec3(0, radius * 3 / 2, 0)));
	addEntity(head);
}

IndexedBox::IndexedBox(GLdouble l)
{
	mMesh = IndexMesh::generateIndexedBox(l);
	setMaterial(glm::vec3(0.0f, 1.0f, 0.0f));
}

bool EntityWithMaterial::mShowNormals = false;

EntityWithMaterial::EntityWithMaterial() {
	mShader = Shader::get("light");
}

void EntityWithMaterial::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		material.upload(*mShader);
		upload(aMat);

		mMesh->render();

		
		if (mShowNormals) {
			Shader* normShader = Shader::get("normals");
			normShader->use();
			normShader->setUniform("modelView", aMat);  // o el nombre que use ese shader
			mMesh->render();
		}
		
	}
}
